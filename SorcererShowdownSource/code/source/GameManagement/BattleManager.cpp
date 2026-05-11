#include "code/header/GameManagement/BattleManager.h"
#include "code/header/GameManagement/BattlefieldHeader.h"
#include "code/header/CharacterCreator/CharacterCreator.h"
#include "code/header/Characters/CharacterList.h"
#include "code/header/Techniques/Limitless.h"
#include "code/header/Characters/Shikigami/ShikigamiList.h"
#include "code/header/Domains/DomainList.h"
#include "code/header/GameManagement/UIDisplay.h"
#include "code/header/GameManagement/Utils.h"

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

void BattleManager::loadSetup(Battlefield& bf, BattleCreator& bc, bool load = false) {
	if (!bc.characterlist.empty()) bc.characterlist.clear();
	bc.characterlist.push_back(std::make_unique<Gojo>());
	bc.characterlist.push_back(std::make_unique<Sukuna>());
	bc.characterlist.push_back(std::make_unique<Yuta>());
	bc.characterlist.push_back(std::make_unique<Toji>());
	bc.characterlist.push_back(std::make_unique<Mahito>());
	bc.characterlist.push_back(std::make_unique<Hakari>());
	if (load) {
		CharacterCreator cc;
		cc.LoadJsonCharacter(bc);
	}
	Character::ResetGlobalID();
	Character::AddGlobalID(int(bf.battlefield.size()));
}

bool BattleManager::SetupBattlefield(Battlefield& bf,BattleCreator& bc) {
	bool choosing = true, spec_mode = false; 
	loadSetup(bf, bc);
	while (choosing) {
		std::println("Choose your sorcerer and the amount of opponents you want to fight!");
		if (!spec_mode) {
			std::println("===> Player: {}", bf.battlefield.empty() ? "None" : bf.battlefield[0]->GetName());
		}
		else {
			std::println("[<Spectator Mode Enabled>]");
		}
		for (auto const& [name, count] : bc.fighter_counts) {
			if (count > 0) std::println("{} x{}", name, count);
		}
		std::println("\n");
		int i = 1;
		for (const auto& s : bc.characterlist) {
			std::println("{}: {}",i, s->GetName());
			i++;
		}
		std::println("-3 - load JSON | -2 - Spectator mode | -1 - Undo | 0 - Finish ");
		
		int c = GetValidInput();

		if (c > 0 && c <= static_cast<int>(bc.characterlist.size())) 
		{
			size_t idx = static_cast<size_t>(c - 1);
			std::unique_ptr<Character> new_character = bc.characterlist[idx]->Clone();
			new_character->AssignID();
			bc.fighter_counts[new_character->GetName()]++;
			bf.battlefield.push_back(std::move(new_character));
		}
		else if (c == 0) 
		{
			if (bf.battlefield.size() < 2) {
				std::println("You need 2 or more sorcerers to start the fight!");
				std::cin.get();
			}
			else {
				choosing = false;
			}
		}
		else if (c == -1 && !bf.battlefield.empty()) 
		{
			bc.fighter_counts[bf.battlefield.back()->GetName()]--;
			bf.battlefield.pop_back();
			Character::AddGlobalID(-1);
		}
		else if (c == -2) 
		{
			spec_mode = !spec_mode;
		}
		else if (c == -3) 
		{
			loadSetup(bf, bc, true);
		}
		else{
			std::println("Invalid Input");
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

	bool player_alive = spectator_mode;
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
		if (c->IsThePlayer()) {
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
	for (const auto& s : bf.battlefield) {
		if (s->IsaCurseUser()) {
			auto curse_user = static_cast<CurseUser*>(s.get());
			if (curse_user->GetDomain() == nullptr) continue;
			if (curse_user->DomainActive()) {
				bf.active_domains.push_back(curse_user);
			}
		}
	}

	if (bf.active_domains.size() > 2) {
		std::println("{}====Its a {}-way domain clash!===={}",Color::BrightMagenta, bf.active_domains.size(), Color::Clear);
		for (const auto& s : bf.active_domains) {
			s->GetDomain()->KillSetDomain(*s, *s->GetDomain());
		}
	}
	else if (bf.active_domains.size() == 2) {
		for (const auto& s : bf.active_domains) {
			s->GetDomain()->SetClashState(true);
		}
		Domain::ClashDomains(*bf.active_domains[0], *bf.active_domains[1]);
	}
	else {
		for (const auto& s : bf.active_domains) {
			if (s->GetDomain()->Clashing()) {
				s->GetDomain()->SetClashState(false);
			}
		}
		if (!bf.active_domains.empty()){
			DoSurehit(bf.active_domains[0], bf);
		}
	}

	for (const auto& s : bf.battlefield) {
		if (s->IsaCurseUser()) {
			auto cr = static_cast<CurseUser*>(s.get());
			if (cr->GetDomain() && cr->DomainActive()){
				cr->TickDomain();
				cr->DomainDrain();
			}
		}
	}
	bf.active_domains.clear();
}

void BattleManager::DoSurehit(CurseUser* crs, Battlefield& bf){
	for (const auto& s : bf.battlefield) {
		if (s.get() == crs) continue;
		std::println("{} has been caught inside of {}'s {}",
			s->GetNameWithID(),
			crs->GetNameWithID(),
			crs->GetDomain()->GetDomainName());
		crs->GetDomain()->OnSureHit(*crs,*s);
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

