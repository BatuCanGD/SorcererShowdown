#include "GameManagement/BattleManager.h"
#include "GameManagement/BattlefieldHeader.h"
#include "GameManagement/UserInterface.h"
#include "GameManagement/Utils.h"
#include "GameManagement/Colors.h"

#include "CharacterCreator/CharacterCreator.h"
#include "CharacterCreator/DomainCreator.h"
#include "CharacterCreator/CursedToolCreator.h"

#include "Characters/CharacterList.h"


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

std::pair<Character*, bool> BattleManager::SetupBattlefield() {
	bool choosing = true, multiple = false, seen = false;
	Character* chosen = nullptr;
	loadSetup(false);
	while (choosing) {
		std::println("Choose your sorcerer and the opponents you want to fight!");
		
		std::println("===> Player: {}", chosen ? chosen->GetName() : "None");
			
		if (!seen){
			std::println("\n[Spectator Mode will trigger if you don't select a character]\n"
			"Choosing a character first is recommended, as the first character added starts the round first.");
			seen = true;
		}

		for (auto const& [name, count] : bc.fighter_counts) {
			std::println("{} x{}", name, count);
		}
		std::println();
		if (multiple) {
			std::println("[<Multiple Additions Enabled>]"); 
		}
	
		int chr = 1;
		for (const auto& s : bc.characterlist) {
			double hp = s->GetCharacterHealth();
			if (s->IsaCurseUser()){ 
				const auto* crs = static_cast<CurseUser*>(s.get());
				const std::string tech = crs->GetTechnique() ? crs->GetTechnique()->GetTechniqueName() : "No Technique";
				const std::string domain = crs->GetDomain() ? crs->GetDomain()->GetDomainName() : "No Domain";
				const std::string counter = crs->GetCounter() ? crs->GetCounter()->GetDomainName() : "No Counter";
				std::println("{}: {} | {} | HP: {:.1f} | CE: {:.1f} | {} | {} | {}", chr, s->GetName(), s->GetType(), hp, crs->GetCharacterMaxCE(), tech, domain, counter);
			}
			else if (s->IsPhysicallyGifted()){
				const auto* pg = static_cast<PhysicallyGifted*>(s.get());
				std::println("{}: {} | {} | HP: {:.1f} | STRENGTH: {:.1f}", chr, s->GetName(),s->GetType(), hp, pg->GetStrength());
			}else{
				std::println("{}: {} | {} | HP: {:.1f}", chr, s->GetName(),s->GetType(), hp);
			}
			chr++;
		}
		SetupChoice(chosen, choosing, multiple);
	}
	return{chosen, chosen ? false : true};
}

void BattleManager::SetupChoice(Character*& chosen, bool& choosing, bool& multi){
	std::println("\n(-5) Clear chosen character | (-4) Choose character   | (-3) Add Multiple Characters\n"
				 "(-2) load Custom Characters | (-11) Clear battlefield | (-1) Undo battlefield addition\n"
				 "-----------------------------</([0 - Finish Setup])\\>--------------------------------\n=> ");
	int c = Utilities::GetInput<int>();

	if (c > 0 && c <= static_cast<int>(bc.characterlist.size())) {
		size_t idx = static_cast<size_t>(c - 1);
		int count = 1;
        if (multi) {
            std::print("How many {}'s do you want to add? ", bc.characterlist[idx]->GetName());
            count = Utilities::GetInput<int>();
        }
        for (int i = 0; i < count; i++) {
            auto new_character = bc.characterlist[idx]->Clone();
            new_character->AssignID();
            bc.fighter_counts[new_character->GetName()]++;
            bf.battlefield.push_back(std::move(new_character));
        }
		UserInterface::ClearScreen();
		return;
	}


	switch(c){
		case 0:
			if (bf.battlefield.size() < 2) {
				std::println("You need 2 or more sorcerers to start the fight!");
				std::cin.get();
			}
			else {
				choosing = false;
			}
			break;
		case -1:
			if (!bf.battlefield.empty()){
				if (chosen == bf.battlefield.back().get()) {
                    chosen = nullptr;
                }
				auto name = bf.battlefield.back()->GetName();
				auto it = bc.fighter_counts.find(name);
				if (it != bc.fighter_counts.end()) {
					if (--it->second <= 0) {
						bc.fighter_counts.erase(it);
					}
				}
				bf.battlefield.pop_back();
				Character::DecrementGlobalID();
				UserInterface::ClearScreen();
			}else{
				std::println("You can't take back actions you didn't take");
			}
			break;
		case -11:
			if (!bf.battlefield.empty()){
				bf.battlefield.clear();
				bc.fighter_counts.clear();
				chosen = nullptr;
				Character::ResetGlobalID();
			}else{
				std::println("The battlefield is empty, there is nothing to clear");
			}
			break;
		case -2:
			loadSetup(true);
			break;
		case -3:
			multi = !multi;
			break;
		case -4: {
            std::print("Select the character you would like to play as\n=> ");
            int p = Utilities::GetInput<int>();
			if (p <= 0 || p > static_cast<int>(bc.characterlist.size())) {
				std::println("Invalid choice");
				break;
			}
			size_t idx = static_cast<size_t>(p - 1);
			const std::string& target_name = bc.characterlist[idx]->GetName();

			auto it = std::find_if(bf.battlefield.begin(), bf.battlefield.end(),
				[&target_name](const auto& f) { return f->GetName() == target_name; 
			});

			if (it != bf.battlefield.end()) {
				chosen = it->get();
				break;
			}

			auto new_character = bc.characterlist[idx]->Clone();
			new_character->AssignID();
			bc.fighter_counts[new_character->GetName()]++;
			chosen = new_character.get();
			bf.battlefield.push_back(std::move(new_character));
			break;
		}
		case -5:
			if (chosen){
				chosen = nullptr;
			}else{
				std::println("You haven't chosen a character");
			}
			break;
		default: 
			std::println("Invalid choice");
	}
	UserInterface::ClearScreen();
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
	return spec_mode ? true : std::any_of(bf.battlefield.begin(), bf.battlefield.end(), [](const auto& s) { return s->IsThePlayer() && s->GetCharacterHealth() > 0.0; });
}

void BattleManager::ManageEndOfTurn(bool minput) {
    std::println("{}=============== TURN AFTERMATH ==============={}", Color::BrightRed, Color::Clear);
	for (const auto& c : bf.battlefield){
		TickCharacters(c.get());
		if (!minput) LogCharacterStatus(c.get());
		ResetCharacterStatus(c.get());
	}
    std::println("{}==================== END OF TURN {} ==================={}\n\n", Color::Yellow, turncount++, Color::Clear);
}

void BattleManager::TickCharacters(Character* c){
	c->TickCharacterSpecialty();
	if (c->IsaCurseUser()) {
		auto* curse_user = static_cast<CurseUser*>(c);
		if (curse_user->IsaSorcerer()) {
			static_cast<Sorcerer*>(curse_user)->TickRCT();
		}
		if (auto* tech = curse_user->GetTechnique()) {
			tech->TickTechnique(curse_user);
			tech->InvulnerabilityNerf(curse_user);
		}
		curse_user->TickShikigami(bf);
		curse_user->TickZone();
		curse_user->RegenCE();
		curse_user->TickBindingVows();
		curse_user->TickReinforcement();
	}
}
void BattleManager::LogCharacterStatus(Character* c){
	if (c->IsaCurseUser()) {
		CurseUser* curse_user = static_cast<CurseUser*>(c);
		double ce_lost = curse_user->GetPreviousCE() - curse_user->GetCharacterCE();
		
		if (ce_lost > 0) {
			std::println("{} {}expended{} {:.1f} {}Cursed Energy{}.", c->GetNameWithID(), Color::Red, Color::Clear, ce_lost, Color::Cyan, Color::Clear);
		} else if (ce_lost < 0) {
			std::println("{} {}gained{} {:.1f} {}Cursed Energy{}.", c->GetNameWithID(), Color::Green, Color::Clear, -ce_lost, Color::Cyan, Color::Clear);
		}
	}
	double hp_lost = c->GetCharacterPreviousHealth() - c->GetCharacterHealth();
	if (hp_lost > 0) {
		std::println("{} took {}{:.1f} damage{} this turn.", c->GetNameWithID(), Color::Red, hp_lost, Color::Clear);
	} else if (hp_lost < 0) {
		std::println("{} {}healed{} {:.1f} health.", c->GetNameWithID(), Color::Green, Color::Clear, -hp_lost);
	}
}
void BattleManager::ResetCharacterStatus(Character* c){
	c->ClearStunTime();  
	c->UpdatePreviousHP();
	if (c->IsaCurseUser()) static_cast<CurseUser*>(c)->UpdatePreviousCE();
}

void BattleManager::DomainCheckAndPerform() {
	std::println("\n\n{}================= END OF TURN SUMMARY ================={}", Color::Yellow, Color::Clear);
	std::println("{}============= DOMAINS AND CLASHES ============{}", Color::BrightMagenta, Color::Clear);
	for (const auto& s : bf.battlefield) {
		if (!s->IsaCurseUser()) continue;
		auto* curse_user = static_cast<CurseUser*>(s.get());

		if (auto* domain = curse_user->GetDomain()) {
			domain->TickDomain(curse_user);
			if (domain->IsActive()){
				bf.active_domains.push_back(curse_user);
			}
		}
		if (auto* counter = curse_user->GetCounter()){
			counter->TickDomain(curse_user);
		}
	}
	if (bf.active_domains.size() > 2) {
		std::println("{}====It's a {}-way domain clash!===={}",Color::BrightMagenta, bf.active_domains.size(), Color::Clear);
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
			std::println("You and everyone else have been wiped off the battlefield, it's a draw!");
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
