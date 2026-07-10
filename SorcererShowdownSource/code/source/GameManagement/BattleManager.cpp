#include "code/header/GameManagement/BattleManager.h"
#include "code/header/GameManagement/BattlefieldHeader.h"
#include "code/header/CharacterCreator/CharacterCreator.h"
#include "code/header/CharacterCreator/DomainCreator.h"
#include "code/header/CharacterCreator/CursedToolCreator.h"
#include "code/header/Characters/CharacterList.h"
#include "code/header/Characters/Shikigami/ShikigamiList.h"
#include "code/header/Domains/DomainList.h"
#include "code/header/GameManagement/UserInterface.h"
#include "code/header/GameManagement/Utils.h"
#include "code/header/GameManagement/Colors.h"

bool BattleManager::GameEndCheck(bool spectator_mode) {
	int alive_sorcerers = 0;
	bool player_found = false;

	for (const auto& sc : bf.battlefield) {
		if (sc->GetCharacterHealth() > 0) {
			alive_sorcerers++;
			if (sc->IsThePlayer()) player_found = true;
		}
	}
	return (!spectator_mode && !player_found) || (alive_sorcerers <= 1);
}

std::tuple<bool, bool, bool> BattleManager::SkipCheck() {
	std::println("Watch the battle turn by turn, skip prompts, or minimize output entirely.");
	if (bf.battlefield.size() >= 100) {
		std::println("[WARNING] Battlefield is filled with more than 100 sorcerers\n"
					 "Minimized output is recommended to speed up turns");
	}
	std::println("-1 - Minimized Output | 0 - Auto-Run (No Prompts) | 1 - Pause After Rounds | 2 - Pause After Turns");
	std::print("=> ");
	int ch = Utilities::GetInput<int>();
	while(ch < -1 || ch > 2){
		
		std::print("=> ");
		ch = Utilities::GetInput<int>();
	}
	UserInterface::ClearScreen();
	return { ch <= 1, ch <= 0, ch == -1 };
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
	Character::SetGlobalID(static_cast<int>(bf.battlefield.size()));
}

bool BattleManager::SetupBattlefield() {
	bool choosing = true, multi_choosing = false, spec_mode = false;
	loadSetup(false);
	while (choosing) {
		std::println("Choose your sorcerer and the opponents you want to fight!");
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
		if (multi_choosing) { std::println("[<Multiple Addition Enabled>]"); }
		int i = 1;
		for (const auto& s : bc.characterlist) {
			double hp = s->GetCharacterHealth();
			if (s->IsaCurseUser()){ auto crs = static_cast<CurseUser*>(s.get());
				auto technigue = crs->GetTechnique() ? crs->GetTechnique()->GetTechniqueName() : "No Technique";
				auto domain = crs->GetDomain() ? crs->GetDomain()->GetDomainName() : "No Domain";
				auto counter = crs->GetCounter() ? crs->GetCounter()->GetDomainName() : "No Counter";
				std::println("{}: {} | {} | HP: {:.1f} | CE: {:.1f} | {} | {} | {}",
					i, s->GetName(), s->GetType(), hp, crs->GetCharacterMaxCE(), technigue, domain, counter);
			}else if (s->IsPhysicallyGifted()){ auto pg = static_cast<PhysicallyGifted*>(s.get());
				std::println("{}: {} | {} | HP: {:.1f} | STRENGTH: {:.1f}",i, s->GetName(),s->GetType(), hp, pg->GetStrength());
			}else{
				std::println("{}: {} | {} | HP: {:.1f}",i, s->GetName(),s->GetType(), hp);
			}
			i++;
		}
		std::println("-4 - Add Multiple | -3 - load JSON | -2 - Spectator mode | -11 - Clear | -1 - Undo | 0 - Finish ");

		int c = Utilities::GetInput<int>();

		if (c > 0 && c <= static_cast<int>(bc.characterlist.size()))
		{
			size_t idx = static_cast<size_t>(c - 1);
			if (multi_choosing) {
				std::print("How many {}'s do you want to add? ", bc.characterlist[c - 1]->GetName());
				int count = Utilities::GetInput<int>();
				for (int j = 0; j < count; j++) {
					std::unique_ptr<Character> new_character = bc.characterlist[idx]->Clone();
					new_character->AssignID();
					bc.fighter_counts[new_character->GetName()]++;
					bf.battlefield.push_back(std::move(new_character));
				}
			}
			else {
				std::unique_ptr<Character> new_character = bc.characterlist[idx]->Clone();
				new_character->AssignID();
				bc.fighter_counts[new_character->GetName()]++;
				bf.battlefield.push_back(std::move(new_character));
				UserInterface::ClearScreen();
			}
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
				Character::DecrementGlobalID();
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
				Character::ResetGlobalID();
			}else{
				std::println("There are no characters in the vector to clear");
			}
		}
		else if (c == -2) spec_mode = !spec_mode;
		else if (c == -3) loadSetup(true);
		else if (c == -4) multi_choosing = !multi_choosing;
		else std::println("Invalid Input");
		UserInterface::ClearScreen();
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
	std::println("{}=============== DEATHS ==============={}", Color::Red, Color::Clear);
	unsigned int deaths = 0;
	for (const auto& s : bf.battlefield) {
		if (s->GetCharacterHealth() <= 0.0) {
			deaths++;
			double taken_damage = s->GetCharacterPreviousHealth() - s->GetCharacterHealth();
			std::println("{} took {}{:.1f}{} damage and is removed from the battlefield!",
				s->GetNameWithID(), Color::Red, taken_damage, Color::Clear);
		}
	}
	if (deaths == 0) std::println("No sorcerers have been killed this turn");
	std::erase_if(bf.battlefield, [](const auto& s) { return s->GetCharacterHealth() <= 0.0; });
}

bool BattleManager::PlayerSearch(bool spec_mode){
	bool player_found = spec_mode;
	for (const auto& s : bf.battlefield){
		if (s->IsThePlayer() && s->GetCharacterHealth() > 0.0) player_found = true;
	}
	return player_found;
}

void BattleManager::ManageEndOfTurn(bool minput) {
    std::println("{}=============== TURN AFTERMATH ==============={}", Color::BrightRed, Color::Clear);
    
    for (const auto& c : bf.battlefield) {
        c->TickCharacterSpecialty();
        if (c->IsaCurseUser()) {
            auto curse_user = static_cast<CurseUser*>(c.get());
            if (auto* tech = curse_user->GetTechnique()) {
				tech->TickTechnique(curse_user);
                tech->InvulnerabilityNerf(curse_user);
            }
            if (curse_user->IsaSorcerer()) static_cast<Sorcerer*>(curse_user)->UseRCT();
            curse_user->TickShikigami(bf);
            curse_user->TickZone();
            curse_user->RegenCE();
            curse_user->TickBindingVows();
            curse_user->TickReinforcement();
        }
    }

    if (!minput || (!minput && bf.battlefield.size() < 100)) {
        for (const auto& c : bf.battlefield) {
            if (c->IsaCurseUser()) {
                auto curse_user = static_cast<CurseUser*>(c.get());
                double ce_lost = curse_user->GetPreviousCE() - curse_user->GetCharacterCE();
                
                if (ce_lost > 0) {
                    std::println("{} {}expended{} {:.1f} {}Cursed Energy{}.", 
                        c->GetNameWithID(), Color::Red, Color::Clear, ce_lost, Color::Cyan, Color::Clear);
                } else if (ce_lost < 0) {
                    std::println("{} {}gained{} {:.1f} {}Cursed Energy{}.", 
                        c->GetNameWithID(), Color::Green, Color::Clear, -ce_lost, Color::Cyan, Color::Clear);
                }
            }
            double hp_lost = c->GetCharacterPreviousHealth() - c->GetCharacterHealth();
            if (hp_lost > 0) {
                std::println("{} took {}{:.1f} damage{} this turn.", 
                    c->GetNameWithID(), Color::Red, hp_lost, Color::Clear);
            } else if (hp_lost < 0) {
                std::println("{} {}healed{} {:.1f} health.", 
                    c->GetNameWithID(), Color::Green, Color::Clear, -hp_lost);
            }
        }
    }

    for (const auto& c : bf.battlefield) {
        if (c->IsaCurseUser()) static_cast<CurseUser*>(c.get())->UpdatePreviousCE();
        c->UpdatePreviousHP();
        if (c->IsCharacterStunned()) c->ClearStunTime();  
    }
    std::println("{}======================================================={}\n"
				 "===================END=OF=TURN={}======================\n\n", Color::Yellow, Color::Clear, turncount++);
}

void BattleManager::DomainCheckAndPerform() {
	std::println("\n\n{}================= END OF TURN SUMMARY ================={}", Color::Yellow, Color::Clear);
	std::println("{}============= DOMAINS AND CLASHES ============{}", Color::BrightMagenta, Color::Clear);
	for (const auto& s : bf.battlefield) {
		if (!s->IsaCurseUser()) continue;
		auto* curse_user = static_cast<CurseUser*>(s.get());

		if (auto* domain = curse_user->GetDomain(); domain && domain->IsActive()) {
			bf.active_domains.push_back(curse_user);
			domain->TickDomain(curse_user);
		}
		if (auto* counter = curse_user->GetCounter(); counter && counter->IsActive()){
			counter->TickDomain(curse_user);
		}
		
	}
	if (bf.active_domains.size() > 2) {
		std::println("{}====Its a {}-way domain clash!===={}",Color::BrightMagenta, bf.active_domains.size(), Color::Clear);
		for (const auto& s : bf.active_domains) {
			s->GetDomain()->EndDomain(s, Domain::EndReason::Collapsed);
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
	if (bf.battlefield.size() >= 100) {
        std::println("Hundreds of fighters are caught inside {}'s {}", 
			crs->GetNameWithID(), crs->GetDomain()->GetDomainName());
        for (const auto& s : bf.battlefield) {
            if (s.get() == crs) continue;
            crs->GetDomain()->OnSureHit(*crs, *s);
        }
        return;
    }
	for (const auto& s : bf.battlefield) {
		if (s.get() == crs) continue;
		std::println("{} has been caught inside of {}'s {}",
			s->GetNameWithID(), crs->GetNameWithID(), 
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
