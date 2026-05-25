#include "code/header/GameManagement/PlayerManager.h"
#include "code/header/BindingVows/BindingVowList.h"
#include "code/header/GameManagement/BattlefieldHeader.h"
#include "code/header/Characters/Shikigami/Shikigami.h"
#include "code/header/Techniques/Techniques.h"
#include "code/header/CursedTools/CursedTool.h"
#include "code/header/Specials/Specials.h"
#include "code/header/Domains/Domain.h"
#include "code/header/Characters/CurseUsers/Sorcerers/Sorcerer.h"
#include "code/header/GameManagement/Utils.h"



void PlayerManager::OnPlayerTurn(Character* s, Battlefield& bf) {
	if (!player_type_found){
		if (s->IsaCurseUser()){
			crs = static_cast<CurseUser*>(s);
			if (s->IsaSorcerer()){
				src = static_cast<Sorcerer*>(s);
			}
		}
		player_type_found = true;
	}
	int player_choice = Utilities::GetInput<int>();
	switch (player_choice) {
	case 1: {
		if (crs) {
			if (!crs->GetTechnique()) {
				std::println("You don't have a technique to use!");
				break;
			}
			else if (crs->GetTechnique()->BurntOut()) {
				std::println("Your technique is burnt out, you cannot use it properly yet");
				break;
			}
		}
		else {
			std::println("You cant use techniques!");
			break;
		}
		Character* target = TargetSelector(bf, s);
		if (target) {
			crs->GetTechnique()->TechniqueMenu(crs, target, bf);
		}
		break;
	}
	case 2: {
		if (Character* target = TargetSelector(bf, s)) {
			std::println("{} engages in close combat with {}!", s->GetNameWithID(), target->GetNameWithID());
			s->Attack(target);
		}
		break;
	}
	case 3: {
		if (crs) {
			if (!crs->GetSpecial()) {
				std::println("You dont have a Special move to use");
				break;
			}
		}
		else {
			std::println("You cant Special Moves");
			break;
		}
		crs->GetSpecial()->PerformSpecial(crs);
		break;
	}
	case 4: {
		if (!crs) {
			std::println("You cant use domains");
			break;
		}
		PlayerDomainUsage(crs);
		break;
	}
	case 5: {
		if (Character* target = TargetSelector(bf, s)) {
			s->Taunt(target);
		}
		break;
	}
	case 6: {
		if (!src) {
			std::println("You cant use Reverse Cursed Technique!");
			break;
		}
		if (src->GetCharacterMaxCE() < 500.0) {
			std::println("You dont have enough cursed energy to continuously maintain Reverse Cursed Technique");
			break;
		}
		PlayerRCTusage(src);
		break;
	}
	case 7:
		if (!crs) {
			std::println("You cant use Domain Amplification.");
			break;
		}
		PlayerDAusage(crs);
		break;
	case 8:
		GetPlayerTools(s);
		break;
	case 9:
		if (crs) {
			if (crs->GetTechnique()) {
				crs->GetTechnique()->TechniqueSetting(crs, bf);
			}
			else {
				std::println("You dont have a Technique");
			}
		}
		else {
			std::println("You cant use Techniques");
		}
		break;
	case 10:
		if (!crs) {
			std::println("You cant use Shikigami");
			break;
		}
		PlayerShikigami(crs);
		break;
	case 11:
		if (!crs) {
			std::println("You cant use Cursed Energy Reinforcement");
			break;
		}
		PlayerReinforcement(crs);
		break;
	case 12:
		if (!crs){
			std::println("You cant use Binding Vows");
			break;
		}
		PlayerVows(crs);
		break;
	default:
		std::println("Invalid Choice");
	}
}

void PlayerManager::PlayerVows(CurseUser* s){
	int num = 0; 
	auto& vows = s->GetBindingVows();
	std::vector<BindingVow*> vs;

	std::println("\n\n****Active Binding Vows****: ");
	for (auto& bv : vows){
		if (bv->IsUnused() || bv->IsUnavailable()) continue;
		std::println("{}: {}", ++num, bv->GetVowDetails());
		vs.push_back(bv.get());
	}
	std::println("\n\n****Available Binding Vows****: ");
	for (auto& bv : vows){
		if (bv->IsActive() || bv->IsUnavailable()) continue;
		std::println("{}: {}", ++num, bv->GetVowDetails());
		vs.push_back(bv.get());
	}
	std::println("\n\n****Unavailable Binding Vows****: ");
	for (auto& bv : vows){
		if (bv->IsActive() || bv->IsUnused()) continue;
		std::println("{}: {}", ++num, bv->GetVowDetails());
		vs.push_back(bv.get());
	}
	std::print("=> ");
	size_t ch = Utilities::GetInput<size_t>();
	if (ch > 0 && ch <= vs.size()){
		std::println("1 - Activate Binding Vow | 2 - Disable Binding Vow (Disabled Forever)");
		vs[ch - 1]->SetVowStatus(Utilities::GetInput<int>());
	}else{
		std::println("Invalid Input");
	}
}

void PlayerManager::PlayerDomainUsage(CurseUser* s) {
	if (!s->GetDomain() && !s->GetCounterDomain()) {
		std::println("You dont have a domain and a counter to a domain");
		return;
	}
	if (s->GetDomain()) {
		std::println("Domain Status: [{}]", s->DomainActive() ? "Active" : "Inactive");
	}
	if (s->GetCounterDomain()) {
		std::println("{} Status: [{}]", s->GetCounterDomain()->GetDomainName(), s->CounterDomainActive() ? "Active" : "Inactive");
	}
	if (s->GetDomain()) {
		std::print("1 - Activate Domain | 2 - Disable Domain ");
	}
	if (s->GetCounterDomain()) {
		std::println("\n3 - Activate {} | 4 - Disable {} ", s->GetCounterDomain()->GetDomainName(), s->GetCounterDomain()->GetDomainName());
	}
	std::print("=> ");
	int ch = Utilities::GetInput<int>();
	switch (ch) {
	case 1:
		if (!s->GetDomain()) {
			std::println("You dont have a domain");
			break;
		}
		s->ActivateDomain();
		break;
	case 2:
		if (!s->GetDomain()) {
			std::println("You dont have a domain");
			break;
		}
		s->DeactivateDomain();
		break;
	case 3:
		if (!s->GetCounterDomain()) {
			std::println("You dont have a counter domain");
			break;
		}
		s->ActivateCounterDomain();
		break;
	case 4:
		if (!s->GetCounterDomain()) {
			std::println("You dont have a counter domain");
			break;
		}
		s->DeactivateCounterDomain();
		break;
	default:
		std::println("Invalid Input");
	}
}

void PlayerManager::GetPlayerTools(Character* s) {
	int count = 1;
	std::println("Available Tools:");
	for (const auto& tool : s->GetCursedTools()) {
		std::println("{} - {}", count++, tool->GetName());
	}

	if (s->GetTool()) {
		std::println("\n0 - Unequip ({})", s->GetTool()->GetName());
	}
	else {
		std::println("\n0 - Go Back");
	}

	std::print("\n=> ");
	size_t choice = Utilities::GetInput<size_t>();
	s->CursedToolChoice(choice);
}

void PlayerManager::PlayerRCTusage(Sorcerer* s) {
	if (!s->HasRCT()){
		std::println("you arent able to use RCT");
		return;
	}
	std::println("1-Enable RCT, 2-Boost RCT, 3-Disable RCT");
	int choice = Utilities::GetInput<int>();
	switch (choice) {
	case 1:
		s->EnableRCT();
		std::println("You have started using RCT");
		break;
	case 2:
		s->BoostRCT();
		std::println("You have started pumping RCT at maximum output");
		break;
	case 3:
		s->DisableRCT();
		std::println("You have disabled RCT");
		break;
	default:
		std::println("Invalid RCT Choice");
	}
}

void PlayerManager::PlayerDAusage(CurseUser* s) {
	std::println("1-On, 2-Off\n=>");
	int choice = Utilities::GetInput<int>();

	switch (choice) {
	case 1:
		s->SetAmplification(true);
		break;
	case 2:
		s->SetAmplification(false);
		break;
	}
}

void PlayerManager::PlayerShikigami(CurseUser* s) {
	if (s->GetShikigami().empty()) {
		std::println("You dont have any shikigami to use");
		return;
	}
	int count = 1;
	for (const auto& sh : s->GetShikigami()) {
		std::println("{}: {} ", count, sh->GetName());
		count++;
	}
	std::println("Choose the shikigami you'd like to use\n=> ");

	size_t ch = Utilities::GetInput<size_t>(); 
	if (ch > 0 && ch <= s->GetShikigami().size()) {
		ch--;
		Shikigami* sk = s->ChooseShikigami(ch);
		std::println("Chosen Shikigami: {} | [{}]", sk->GetName(), sk->GetShikigamiStatus());
		if (!sk->IsActivePhysically()) {
			std::println("1 - Manifest");
		}
		if (!sk->IsPartiallyActive()) {
			std::println("2 - Partially Manifest");
		}
		if (sk->IsActive()) {
			std::println("3 - Dismiss");
		}
		std::println("0 - Cancel");
		std::print("=> ");

		int cs = Utilities::GetInput<int>();
		switch (cs) {
		case 1:
			if (sk->IsActivePhysically()) {
				std::println("{} is already physically present!", sk->GetName());
			}
			else {
				sk->Manifest();
				std::println("{} has been Physically Manifested on the battlefield!", sk->GetName());
			}
			break;
		case 2:
			if (sk->IsPartiallyActive()) {
				std::println("{} is already active!", sk->GetName());
			}
			else {
				sk->PartiallyManifest();
				std::println("{}'s technique is now Projected. [Ability Active]", sk->GetName());
			}
			break;
		case 3:
			if (!sk->IsActive()) {
				std::println("{} is already dormant.", sk->GetName());
			}
			else {
				sk->Withdraw();
				std::println("{} has been dismissed and is now Dormant.", sk->GetName());
			}
			break;
		case 0:
			std::println("Action cancelled.");
			break;

		default:
			std::println("Invalid input.");
			break;
		}

	}
	else {
		std::println("Invalid Choice");
	}
}

void PlayerManager::PlayerReinforcement(CurseUser* s) {
	std::println("more reinforcement means harder hit to your CE spending");
	std::println("Current: {}", s->GetReinforcementStatus());
	std::println("1 - Add reinforcement amount 2 - Subtract reinforcement amount  3 - Set reinforcement amount");
	std::print("=> "); int ch = Utilities::GetInput<int>();
	if (ch >= 1 && ch <= 3){
		if (ch == 1) {
			std::println("\nWrite out the amount you would like to reinforce by");
			
		}
		else if (ch == 2) {
			std::println("\nWrite out the amount you would like reduce reinforcement by");
		}
		else if (ch == 3) {
			std::println("\nWrite out the amount that you would like to set the reinforcement to");
		}
		std::print("=> ");
		double c = Utilities::GetInput<double>();
		switch(ch){
		case 1:
			s->AddReinforcement(c);
			break;
		case 2:
			s->AddReinforcement(-c);
			break;
		case 3:
			s->SetCurrentReinforcement(c);		
			break;
		}
	}

}

Character* PlayerManager::TargetSelector(Battlefield& bf, Character* player) {
	std::println("Choose your target:");
	for (size_t i = 0; i < bf.battlefield.size(); ++i) {
		auto& current = *bf.battlefield[i];
		double health = current.GetCharacterHealth();
		double cursed_energy = 0;
		Technique* tech = nullptr;
		Domain* domain = nullptr;

		std::string t_status = "";
		std::string d_status = "";

		if (current.IsaCurseUser()){
			auto cr = static_cast<CurseUser*>(&current);
			tech = cr->GetTechnique();
			domain = cr->GetDomain();
			cursed_energy = cr->GetCharacterCE();
			t_status = (tech == nullptr) ? "" : std::format("| Technique status: [{}] ", tech->GetStringStatus());
			d_status = (domain == nullptr) ? "" : std::format("| Domain status: [{}] ", cr->GetDomainStatus());
		}
		std::string stunned = current.IsCharacterStunned() ? " (Stunned)" : "";
		std::string name = current.GetName();
		std::string ce_display = current.IsPhysicallyGifted() ? "Heavenly Restricted" : std::format("{:.1f} CE", cursed_energy);
		if (bf.battlefield[i].get()->IsThePlayer()) {
			std::println("{}: {} (You)",
							i, name);
			continue;
		}
		std::println("{}: {}{} | ({:.1f} HP) ({}) {}{} ",
					i, name, stunned, health, ce_display, t_status, d_status);
	}

	std::print("=> ");
	size_t t;
	if (!(std::cin >> t) || t >= bf.battlefield.size() || bf.battlefield[t].get() == player || bf.battlefield[t]->GetCharacterHealth() <= 0.0) {
		if (std::cin.fail()) { 
			std::cin.clear(); 
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
		}
		std::println("Target missed or invalid!");
		return nullptr;
	}
	return bf.battlefield[t].get();
}