#include "code/header/GameManagement/BattleManager.h"
#include "code/header/GameManagement/BattlefieldHeader.h"
#include "code/header/CharacterCreator/CharacterCreator.h"
#include "code/header/CharacterCreator/DomainCreator.h"
#include "code/header/CharacterCreator/CursedToolCreator.h"
#include "code/header/Characters/CharacterList.h"
#include "code/header/Techniques/Limitless/Limitless.h"
#include "code/header/Characters/Shikigami/ShikigamiList.h"
#include "code/header/Domains/DomainList.h"
#include "code/header/GameManagement/UserInterface.h"
#include "code/header/GameManagement/Utils.h"

bool BattleManager::GameEndCheck(bool spectator_mode) {
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

std::pair<bool, bool> BattleManager::SkipTurnFullyCheck() {
	std::println("Watch the battle turn by turn, or skip to the end of the round?");
	std::println("0 - Skip Everything  |  1 - Skip AI turns  |  2 - Watch AI turns");
	std::print("=> ");
	int ch = Utilities::GetInput<int>();
	while(ch < 0 || ch > 2){
		std::println("Invalid input");
		std::print("=> ");
		ch = Utilities::GetInput<int>();
	}
	UserInterface::ClearScreen();
	return { ch <= 1, ch == 0 };
}

void BattleManager::loadSetup(bool load) {
	if (!bc.characterlist.empty()) bc.characterlist.clear();
	bc.characterlist.push_back(std::make_unique<Dummy>());
	bc.characterlist.push_back(std::make_unique<Gojo>());
	bc.characterlist.push_back(std::make_unique<Sukuna>());
	bc.characterlist.push_back(std::make_unique<Yuta>());
	bc.characterlist.push_back(std::make_unique<Hakari>());
	bc.characterlist.push_back(std::make_unique<Mahito>());
	bc.characterlist.push_back(std::make_unique<Toji>());
	if (load) {
		if (!bc.domainlist.empty()) bc.domainlist.clear();
		if (!bc.cursedtoollist.empty()) bc.cursedtoollist.clear();
		CursedToolCreator ctc; DomainCreator dc; CharacterCreator cc;
		ctc.LoadJsonObject(bc); dc.LoadJsonObject(bc); cc.LoadJsonObject(bc);
	}
	Character::ResetGlobalID();
	Character::AddGlobalID(static_cast<int>(bf.battlefield.size()));
}

bool BattleManager::SetupBattlefield() {
	bool choosing = true, spec_mode = false; 
	loadSetup(false);
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
			double hp = s->GetCharacterHealth();
			if (s->IsaCurseUser()){ auto crs = static_cast<CurseUser*>(s.get());
				auto technigue = crs->GetTechnique() ? crs->GetTechnique()->GetTechniqueName() : "No Technique";
				auto domain = crs->GetDomain() ? crs->GetDomain()->GetDomainName() : "No Domain";
				auto counter = crs->GetCounterDomain() ? crs->GetCounterDomain()->GetDomainName() : "No Counter";
				std::println("{}: {} | {} | HP: {:.1f} | CE: {:.1f} | {} | {} | {}",
					i, s->GetName(), s->GetType(), hp, crs->GetCharacterMaxCE(), technigue, domain, counter);
			}else if (s->IsPhysicallyGifted()){ auto pg = static_cast<PhysicallyGifted*>(s.get());
				std::println("{}: {} | {} | HP: {:.1f} | STRENGTH: {:.1f}",i, s->GetName(),s->GetType(), hp, pg->GetStrength());
			}else{
				std::println("{}: {} | {} | HP: {:.1f}",i, s->GetName(),s->GetType(), hp);
			}
			i++;
		}
		std::println("-3 - load JSON | -2 - Spectator mode | -11 - Clear | -1 - Undo | 0 - Finish ");
		
		int c = Utilities::GetInput<int>();

		if (c > 0 && c <= static_cast<int>(bc.characterlist.size())) 
		{
			size_t idx = static_cast<size_t>(c - 1);
			std::unique_ptr<Character> new_character = bc.characterlist[idx]->Clone();
			new_character->AssignID();
			bc.fighter_counts[new_character->GetName()]++;
			bf.battlefield.push_back(std::move(new_character));
			UserInterface::ClearScreen();
		}
		else if (c == 0) 
		{
			if (bf.battlefield.size() < 2) {
				std::println("You need 2 or more sorcerers to start the fight!");
				std::cin.get();
			}
			else {
				choosing = false;
				UserInterface::ClearScreen();
			}
		}
		else if (c == -1) 
		{
			if (!bf.battlefield.empty()){
				bc.fighter_counts[bf.battlefield.back()->GetName()]--;
				bf.battlefield.pop_back();
				Character::AddGlobalID(-1);
				UserInterface::ClearScreen();
			}else{
				std::println("There are no characters left to undo");
			}
		}
		else if(c == -11)
		{
			if (!bf.battlefield.empty()){
				bf.battlefield.clear();
				bc.fighter_counts.clear();
			}else{
				std::println("There are no characters in the vector to clear");
			}
		}
		else if (c == -2) 
		{
			spec_mode = !spec_mode;
			UserInterface::ClearScreen();
		}
		else if (c == -3) 
		{
			loadSetup(true);
		}
		else{
			std::println("Invalid Input");
		}
	}
	return spec_mode;
}

void BattleManager::SpawnNewFighters() {
	for (auto& new_unit : bf.spawn_queue) {
		new_unit->AssignID();
		bf.battlefield.push_back(std::move(new_unit));
	}
	bf.spawn_queue.clear();
}

void BattleManager::ClearCharacters(){
	std::println("{}=============== DEATHS ==============={}", Utilities::Color::Red, Utilities::Color::Clear);
	for (const auto& s : bf.battlefield) {
		if (s->GetCharacterHealth() <= 0.0) {
			double taken_damage = s->GetCharacterPreviousHealth() - s->GetCharacterHealth();
			std::println("{} took {}{:.1f}{} damage and is removed from the battlefield!",
				s->GetNameWithID(), Utilities::Color::Red, taken_damage, Utilities::Color::Clear);
		}
	}
	std::erase_if(bf.battlefield, [](const auto& s) { return s->GetCharacterHealth() <= 0.0; });
}

bool BattleManager::PlayerSearch(bool spec_mode){
	bool player_found = spec_mode;
	for (const auto& s : bf.battlefield){
		if (s->IsThePlayer() && s->GetCharacterHealth() > 0.0) player_found = true;
	}
	return player_found;
}

void BattleManager::ManageEndOfTurn() { 
	std::println("{}=============== TURN AFTERMATH ==============={}", Utilities::Color::BrightRed, Utilities::Color::Clear);
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
			curse_user->TickBindingVows();
			curse_user->TickReinforcement();
			double current_ce = curse_user->GetCharacterCE();
			if (current_ce < ce_before_regen) {
				double ce_spent = ce_before_regen - current_ce;
				std::println("{} {}expended{} {:.1f} {}Cursed Energy{} this turn.", c->GetNameWithID(),Utilities::Color::Red,Utilities::Color::Clear, ce_spent, Utilities::Color::Cyan, Utilities::Color::Clear);
			}
			else if (current_ce > ce_before_regen) {
				double ce_gained = current_ce - ce_before_regen;
				std::println("{} {}gained{} {:.1f} {}Cursed Energy{} this turn.", c->GetNameWithID(),Utilities::Color::Green,Utilities::Color::Clear, ce_gained, Utilities::Color::Cyan, Utilities::Color::Clear);
			}
			curse_user->UpdatePreviousCE();
		}
		c->TickCharacterSpecialty();
		double total_damage = c->GetCharacterPreviousHealth() - health_before_regen;
		double healed_amount = c->GetCharacterHealth() - health_before_regen;
		if (total_damage > 0) {
			std::println("{} took {}{:.1f} damage{} this turn", c->GetNameWithID(), Utilities::Color::Red, total_damage, Utilities::Color::Clear);
			if (c->GetCharacterHealth() >= c->GetCharacterPreviousHealth()) {
				std::println("{} {}healed the damage back!{}", c->GetNameWithID(), Utilities::Color::Green, Utilities::Color::Clear);
			}
			else if (healed_amount > 0) {
				std::println("{} {}partially healed their wounds.{}", c->GetNameWithID(), Utilities::Color::Yellow, Utilities::Color::Clear);
			}
		}
		c->UpdatePreviousHP();
		if (c->IsCharacterStunned()){
			c->ClearStunTime();
		}
	}
	std::println("{}======================================================={}", Utilities::Color::Yellow, Utilities::Color::Clear);
}

void BattleManager::DomainCheckAndPerform() {
	std::println("\n\n{}================= END OF TURN SUMMARY ================={}", Utilities::Color::Yellow, Utilities::Color::Clear);
	std::println("{}============= DOMAINS AND CLASHES ============{}", Utilities::Color::BrightMagenta, Utilities::Color::Clear);
	for (const auto& s : bf.battlefield) {
		if (s->IsaCurseUser()) {
			auto curse_user = static_cast<CurseUser*>(s.get());
			if (curse_user->GetDomain() && curse_user->DomainActive()) {
				bf.active_domains.push_back(curse_user);
			}
		}
	}
	for (const auto& s : bf.active_domains) {
        s->TickDomain();
        s->DomainDrain();
    }
	if (bf.active_domains.size() > 2) {
		std::println("{}====Its a {}-way domain clash!===={}",Utilities::Color::BrightMagenta, bf.active_domains.size(), Utilities::Color::Clear);
		for (const auto& s : bf.active_domains) {
			s->GetDomain()->ResetDomain(*s, *s->GetDomain());
		}
	}
	else if (bf.active_domains.size() == 2) {
		Domain::ClashDomains(*bf.active_domains[0], *bf.active_domains[1]);
	}
	else if (bf.active_domains.size() == 1){
		DoSurehit(bf.active_domains[0]);
	}else{
		std::println("No domains are active this turn");
	}
	bf.active_domains.clear();
}

void BattleManager::DoSurehit(CurseUser* crs){
	for (const auto& s : bf.battlefield) {
		if (s.get() == crs) continue;
		std::println("{} has been caught inside of {}'s {}",
			s->GetNameWithID(),
			crs->GetNameWithID(),
			crs->GetDomain()->GetDomainName());
		crs->GetDomain()->OnSureHit(*crs,*s);
	}
}

bool BattleManager::IsBattleOver(bool game_over, bool player_found, bool spectator_mode) {
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
	if (bf.battlefield.size() == 1) {
		if (bf.battlefield[0]->IsThePlayer()) {
			std::println("\nCongratulations! You have defeated all other sorcerers and won the battle!");
		} else {
			std::println("\nYou have been defeated by {}! Game Over.", bf.battlefield[0]->GetNameWithID());
		}
		return true;
	}
	if (!player_found) {
		std::println("\nYou have been defeated! The battle rages on without you. Game Over.");
		return true;
	}
	return false;
}

bool BattleManager::GameEndChoice(){
	std::println("1 - End Game | 2 - Restart");
	return Utilities::GetInput<int>() == 1;
}