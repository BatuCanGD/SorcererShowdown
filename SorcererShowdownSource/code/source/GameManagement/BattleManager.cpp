#include "BattleManager.h"
#include "BattlefieldHeader.h"
#include "CharacterCreator.h"
#include "CharacterList.h"
#include "Limitless.h"
#include "ShikigamiList.h"
#include "DomainList.h"
#include "UIDisplay.h"
#include "Utils.h"

bool BattleManager::GameEndCheck(Battlefield& bf, bool spectator_mode) {
	int alive_sorcerers = 0;
	bool player_found = false;

	for (const auto& sc : bf.battlefield) {
		if (sc->GetCharacterHealth() > 0) {
			alive_sorcerers++;
			if (sc->IsThePlayer()) player_found = true;
		}
	}

	if (!spectator_mode && !player_found) return true;
	if (alive_sorcerers <= 1) return true;
	return false;
}

bool BattleManager::SkipTurnFullyCheck() {
	std::println("1 - Skip AI sorcerer turns\n"
				 "2 - Keep AI sorcerer turns");
	std::println("this is for if you want to read what every sorcerer on the board does line by line\n"
				 "or if you want to skip to do a days worth of simulation");
	std::print("=> ");
	int ch = GetValidInput();
	UserInterface::ClearScreen();
	switch (ch) {
	case 1: return true;
	case 2: return false;
	default: return false;
	}
}

void BattleManager::loadSetup(Battlefield& bf, bool load = false) {
	if (!bf.characterlist.empty()) bf.characterlist.clear();
	bf.characterlist.push_back(std::make_unique<Gojo>());
	bf.characterlist.push_back(std::make_unique<Sukuna>());
	bf.characterlist.push_back(std::make_unique<Yuta>());
	bf.characterlist.push_back(std::make_unique<Toji>());
	bf.characterlist.push_back(std::make_unique<Mahito>());
	bf.characterlist.push_back(std::make_unique<Hakari>());
	if (load) {
		CharacterCreator cc;
		cc.LoadJsonCharacter(bf);
	}
	Character::ResetGlobalID();
	Character::AddGlobalID(int(bf.battlefield.size()));
}

bool BattleManager::SetupBattlefield(Battlefield& bf) {
	bool choosing = true, spec_mode = false; 
	int c = 0;
	
	loadSetup(bf);

	while (choosing) {
		std::println("Choose your sorcerer and the amount of opponents you want to fight!");
		if (!spec_mode) {
			std::println("===> Player: {}", bf.battlefield.empty() ? "None" : bf.battlefield[0]->GetName());
		}
		else {
			std::println("[<Spectator Mode Enabled>]");
		}
		for (auto const& [name, count] : bf.fighter_counts) {
			if (count > 0) std::println("{} x{}", name, count);
		}
		std::println("\n");
		int i = 1;
		for (const auto& s : bf.characterlist) {
			std::println("{}: {}",i, s->GetName());
			i++;
		}
		std::println("-3 - load JSON | -2 - Spectator mode | -1 - Undo | 0 - Finish ");
		
		if (!(std::cin >> c)) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			continue;
		}

		if (c == 0) {
			if (bf.battlefield.size() < 2) {
				std::println("You need 2 or more sorcerers to start the fight!");
				std::cin.ignore();
				std::cin.get();
			}
			else {
				choosing = false;
			}
		}
		else if (c == -1 && !bf.battlefield.empty()) {
			bf.fighter_counts[bf.battlefield.back()->GetName()]--;
			bf.battlefield.pop_back();
			Character::AddGlobalID(-1);
		}
		else if (c == -2) {
			if (spec_mode) {
				spec_mode = false;
			}
			else {
				spec_mode = true;
			}
		}
		else if (c == -3) {
			loadSetup(bf, true);
		}
		else {
			if (c > 0 && c <= bf.characterlist.size()) {
				int index = c - 1;
				std::unique_ptr<Character> new_character = bf.characterlist[index]->Clone();
				new_character->AssignID();
				bf.fighter_counts[new_character->GetName()]++;
				bf.battlefield.push_back(std::move(new_character));
			}
			else {
				std::println("Invalid selection!");
			}
		}
		UserInterface::ClearScreen();
	}
	return spec_mode;
}

void BattleManager::SpawnNewFighters(Battlefield& bf) {
	for (auto& new_unit : bf.spawn_queue) {
		new_unit->AssignID();
		bf.battlefield.push_back(std::move(new_unit));
	}
	bf.spawn_queue.clear();
}

bool BattleManager::ManageEndOfTurn(Battlefield& bf, bool spectator_mode) {
	std::println("{}=============== TURN AFTERMATH ==============={}", Color::BrightRed, Color::Clear);

	auto [removed_begin, removed_end] = std::ranges::remove_if(bf.battlefield, [](const auto& s) {
		if (s->GetCharacterHealth() <= 0.0) {
			std::println("{} has been {}defeated and is removed from the battlefield!{}",s->GetNameWithID(), Color::Red,  Color::Clear);
			return true;
		}
		return false;
		});
	bf.battlefield.erase(removed_begin, removed_end);

	bool player_alive = false;

	for (const auto& c : bf.battlefield) {
		double health_before_regen = c->GetCharacterHealth();
		if (c->IsaCurseUser()) {
			auto curse_user = static_cast<CurseUser*>(c.get());
			double ce_before_regen = curse_user->GetCharacterCE();
			if (auto* tech = curse_user->GetTechnique()) {
				if (tech->IsLimitless()) {
					auto lim = static_cast<Limitless*>(tech);
					lim->InfinityNerf(curse_user);
				}
			}
			if (curse_user->IsaSorcerer()) {
				auto sorcerer = static_cast<Sorcerer*>(curse_user);
				sorcerer->UseRCT();
			}
			curse_user->TickShikigami(bf);
			curse_user->RecoverBurnout();
			curse_user->RecoverTechniqueBurnout(curse_user->GetTechnique());
			curse_user->TickZone();
			curse_user->RegenCE();
			curse_user->TickReinforcement();

			double current_ce = curse_user->GetCharacterCE();
			double total_ce = curse_user->GetCharacterPreviousCE();
			if (current_ce < ce_before_regen) {
				double ce_spent = ce_before_regen - current_ce;
				std::println("{} {}expended{} {:.1f} {}Cursed Energy{} this turn.", c->GetNameWithID(),Color::Red,Color::Clear, ce_spent, Color::Cyan, Color::Clear);
			}
			else if (current_ce > ce_before_regen) {
				double ce_gained = current_ce - ce_before_regen;
				std::println("{} {}gained{} {:.1f} {}Cursed Energy{} this turn.", c->GetNameWithID(),Color::Green,Color::Clear, ce_gained, Color::Cyan, Color::Clear);
			}
			curse_user->UpdatePreviousCE();
		}
		c->TickCharacterSpecialty();
		double total_damage = c->GetCharacterPreviousHealth() - health_before_regen;
		double healed_amount = c->GetCharacterHealth() - health_before_regen;
		if (total_damage > 0) {
			std::println("{} took {}{:.1f} damage{} this turn", c->GetNameWithID(), Color::Red, total_damage, Color::Clear);
			if (c->GetCharacterHealth() >= c->GetCharacterPreviousHealth()) {
				std::println("{} {}healed the damage back!{}", c->GetNameWithID(), Color::Green, Color::Clear);
			}
			else if (healed_amount > 0) {
				std::println("{} {}partially healed their wounds.{}", c->GetNameWithID(), Color::Yellow, Color::Clear);
			}
		}
		if (c->IsThePlayer() || spectator_mode) {
			player_alive = true;
		}
		c->UpdatePreviousHP();
		c->ClearStunTime();
	}
	std::println("{}======================================================={}", Color::Yellow, Color::Clear);
	return player_alive;
}

void BattleManager::DomainCheckAndPerform(Battlefield& bf) {
	std::println("\n\n{}================= END OF TURN SUMMARY ================={}", Color::Yellow, Color::Clear); // this is here now because its just 1 line away from manage end of turn
	std::println("{}============= DOMAINS AND CLASHES ============{}", Color::BrightMagenta, Color::Clear);
	std::vector<CurseUser*> active_domains;

	for (const auto& s : bf.battlefield) {
		if (s->IsaCurseUser()) {
			auto curse_user = static_cast<CurseUser*>(s.get());
			if (curse_user->GetDomain() == nullptr) continue;
			if (curse_user->DomainActive()) {
				active_domains.push_back(curse_user);
			}
		}
	}

	for (auto s : active_domains) {
		s->DomainDrain();
	}

	if (active_domains.size() > 2) {
		std::println("{}====Its a {}-way domain clash!===={}",Color::BrightMagenta, active_domains.size(), Color::Clear);
		for (const auto& s : active_domains) {
			s->GetDomain()->KillSetDomain(*s, *s->GetDomain());
		}
	}
	else if (active_domains.size() == 2) {
		for (const auto& s : active_domains) {
			s->GetDomain()->SetClashState(true);
		}
		active_domains[0]->GetDomain()->ClashDomains(*active_domains[0], *active_domains[1]);
	}
	else {
		for (const auto& s : active_domains) {
			if (s->GetDomain()->Clashing()) {
				s->GetDomain()->SetClashState(false);
			}
		}
	}
	///// DOMAIN EFFECTS, WITHOUT CLASH /////
	if (active_domains.size() == 1) {
		CurseUser* domain_user = active_domains[0];

		for (const auto& s : bf.battlefield) {
			if (s.get() == domain_user) continue;
			std::println("{} has been caught inside of {}'s {}",
				s->GetNameWithID(),
				domain_user->GetNameWithID(),
				domain_user->GetDomain()->GetDomainName());
			domain_user->GetDomain()->OnSureHit(*domain_user,*s);
		}
	}
	for (const auto& s : bf.battlefield) {
		if (s->IsaCurseUser()) {
			auto cr = static_cast<CurseUser*>(s.get());
			cr->TickDomain();
		}
	}
}

bool BattleManager::IsBattleOver(bool game_over ,bool player_found,bool spectator_mode, Battlefield& bf) {
	if (!game_over && bf.battlefield.size() > 1 && (player_found || spectator_mode)) return false;

	if (bf.battlefield.empty()) {
		if (spectator_mode) {
			std::println("Every sorcerer has been wiped off the battlefield!");
		}
		else {
			std::println("You and everyone else has been wiped off the battlefield, it's a draw!");
		}
		return true;
	}
	if (spectator_mode) {
		if (bf.battlefield.size() == 1) {
			std::println("The battle has ended, {} is the last one standing!", bf.battlefield[0]->GetNameWithID());
		}
		return true;
	}
	if (!player_found) {
		if (bf.battlefield.size() == 1) {
			std::println("\nYou have been defeated by {}! Game Over.", bf.battlefield[0]->GetNameWithID());
		}
		else {
			std::println("\nYou have been defeated! The battle rages on without you. Game Over.");
		}
		return true;
	}
	if (player_found && bf.battlefield.size() == 1) {
		std::println("\nCongratulations! You have defeated all other sorcerers and won the battle!");
		return true;
	}
	return false;
}

