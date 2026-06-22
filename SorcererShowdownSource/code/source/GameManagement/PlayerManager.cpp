#include "code/header/GameManagement/PlayerManager.h"
#include "code/header/BindingVows/BindingVowList.h"
#include "code/header/GameManagement/BattlefieldHeader.h"
#include "code/header/Characters/Shikigami/Shikigami.h"
#include "code/header/Techniques/Techniques.h"
#include "code/header/CursedTools/CursedTool.h"
#include "code/header/Specials/Specials.h"
#include "code/header/Domains/Domain.h"
#include "code/header/Characters/CurseUsers/Sorcerers/Sorcerer.h"
#include "code/header/GameManagement/Colors.h"
#include "code/header/GameManagement/Utils.h"

void PlayerType::FindPlayerType(){
	if (!player) return;
	if (player->IsaCurseUser()){
		crs = static_cast<CurseUser*>(player);
		if (crs->IsaSorcerer()){
			src = static_cast<Sorcerer*>(crs);
		}
	}
}

void PlayerManager::OnPlayerTurn(Battlefield& bf) {
	int player_choice = Utilities::GetInput<int>();
	while (player_choice < 1 || player_choice > 12){ 
		std::println("Invalid Input!");
		player_choice = Utilities::GetInput<int>(); 
	}
	switch (player_choice) {
	case 1: {
		if (pt.crs) {
			if (!pt.crs->GetTechnique()) {
				std::println("You don't have a technique to use!");
				break;
			}
			else if (pt.crs->GetTechnique()->BurntOut()) {
				std::println("Your technique is burnt out, you cannot use it properly yet");
				break;
			}
		}
		else {
			std::println("You cant use techniques!");
			break;
		}
		Character* target = TargetSelector(bf);
		if (target) {
			pt.crs->GetTechnique()->TechniqueMenu(pt.crs, target, bf);
		}
		break;
	}
	case 2: {
		if (Character* target = TargetSelector(bf)) {
			std::println("{} engages in close combat with {}!", pt.player->GetNameWithID(), target->GetNameWithID());
			pt.player->Attack(target);
		}
		break;
	}
	case 3: {
		if (pt.crs) {
			if (!pt.crs->GetSpecial()) {
				std::println("You dont have a Special move to use");
				break;
			}
		}
		else {
			std::println("You cant Special Moves");
			break;
		}
		pt.crs->GetSpecial()->PerformSpecial(pt.crs);
		break;
	}
	case 4: {
		if (!pt.crs) {
			std::println("You cant use domains");
			break;
		}
		PlayerDomainUsage();
		break;
	}
	case 5: {
		if (Character* target = TargetSelector(bf)) {
			pt.player->Taunt(target);
		}
		break;
	}
	case 6: {
		if (!pt.src) {
			std::println("You cant use Reverse Cursed Technique!");
			break;
		}
		if (pt.src->GetCharacterMaxCE() < 500.0) {
			std::println("You dont have enough cursed energy to continuously maintain Reverse Cursed Technique");
			break;
		}
		PlayerRCTusage();
		break;
	}
	case 7:
		if (!pt.crs) {
			std::println("You cant use Domain Amplification.");
			break;
		}
		PlayerDAusage();
		break;
	case 8:
		GetPlayerTools();
		break;
	case 9:
		if (pt.crs) {
			if (pt.crs->GetTechnique()) {
				pt.crs->GetTechnique()->TechniqueSetting(pt.crs, bf);
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
		if (!pt.crs) {
			std::println("You cant use Shikigami");
			break;
		}
		PlayerShikigami();
		break;
	case 11:
		if (!pt.crs) {
			std::println("You cant use Cursed Energy Reinforcement");
			break;
		}
		PlayerReinforcement();
		break;
	case 12:
		if (!pt.crs){
			std::println("You cant use Binding Vows");
			break;
		}
		PlayerVows();
		break;
	default:
		std::println("Invalid Choice");
	}
}

void PlayerManager::PlayerVows() {
    auto& player_vows = pt.crs->GetBindingVows();
    const auto& binding_vows = BindingVow::GetBindingVows();
    int num = 0; std::vector<BindingVow*> vowlist;
    
    std::println("\n\n****{}Active Binding Vows{}****: ", Color::Green, Color::Clear);
    for (auto& p : player_vows) {
        std::println("{}: {}", ++num, p->GetVowDetails());
        vowlist.push_back(p.get());
    }
    std::println("\n\n****{}Available Binding Vows{}****: ", Color::Yellow, Color::Clear);
    for (auto& bv : binding_vows) {
        bool already_active = false;
        for (auto& p : player_vows) {
            if (p->GetVowDetails() == bv->GetVowDetails()) {
                already_active = true;
                break;
            }
        }
        if (already_active) continue;
        std::println("{}: {}", ++num, bv->GetVowDetails());
        vowlist.push_back(bv.get());
    }
    std::print("=> ");

    size_t ch = Utilities::GetInput<size_t>();
    if (ch == 0 || ch > vowlist.size()) {
        std::println("Invalid choice.");
        return;
    }

    BindingVow* chosen = vowlist[ch - 1];
    bool is_in_inventory = false;
    size_t inv = 0;
    for (size_t i = 0; i < player_vows.size(); i++) {
        if (player_vows[i].get() == chosen) {
            is_in_inventory = true;
            inv = i;
            break;
        }
    }

    if (is_in_inventory) {
        std::println("1 - Remove Binding Vow | 0 - Cancel");
        int c = Utilities::GetInput<int>();
        if (c == 1) {
            pt.crs->RemoveBindingVow(inv);
            std::println("Binding Vow removed.");
        }
    } else {
        std::println("1 - Use Binding Vow | 0 - Cancel");
        int c = Utilities::GetInput<int>();
        if (c == 1) {
            pt.crs->AddBindingVow(chosen->Clone());
            std::println("Binding Vow added!");
        }
    }
}

void PlayerManager::PlayerDomainUsage() {
	if (!pt.crs->GetDomain() && !pt.crs->GetCounterDomain()) {
		std::println("You dont have a domain and a counter to a domain");
		return;
	}
	if (pt.crs->GetDomain()) {
		std::println("Domain Status: [{}]", pt.crs->DomainActive() ? "Active" : "Inactive");
	}
	if (pt.crs->GetCounterDomain()) {
		std::println("{} Status: [{}]", pt.crs->GetCounterDomain()->GetDomainName(), pt.crs->CounterDomainActive() ? "Active" : "Inactive");
	}
	if (pt.crs->GetDomain()) {
		std::print("1 - Activate Domain | 2 - Disable Domain ");
	}
	if (pt.crs->GetCounterDomain()) {
		std::println("\n3 - Activate {} | 4 - Disable {} ", pt.crs->GetCounterDomain()->GetDomainName(), pt.crs->GetCounterDomain()->GetDomainName());
	}
	std::print("=> ");
	int ch = Utilities::GetInput<int>();
	switch (ch) {
	case 1:
		if (!pt.crs->GetDomain()) {
			std::println("You dont have a domain");
			break;
		}
		pt.crs->ActivateDomain();
		break;
	case 2:
		if (!pt.crs->GetDomain()) {
			std::println("You dont have a domain");
			break;
		}
		pt.crs->DeactivateDomain();
		break;
	case 3:
		if (!pt.crs->GetCounterDomain()) {
			std::println("You dont have a counter domain");
			break;
		}
		pt.crs->ActivateCounterDomain();
		break;
	case 4:
		if (!pt.crs->GetCounterDomain()) {
			std::println("You dont have a counter domain");
			break;
		}
		pt.crs->DeactivateCounterDomain();
		break;
	default:
		std::println("Invalid Input");
	}
}

void PlayerManager::GetPlayerTools() {
	int count = 1;
	std::println("Available Tools:");
	for (const auto& tool : pt.player->GetCursedTools()) {
		std::println("{} - {}", count++, tool->GetName());
	}

	if (pt.player->GetTool()) {
		std::println("\n0 - Unequip ({})", pt.player->GetTool()->GetName());
	}
	else {
		std::println("\n0 - Go Back");
	}

	std::print("\n=> ");
	size_t choice = Utilities::GetInput<size_t>();
	pt.player->CursedToolChoice(choice);
}

void PlayerManager::PlayerRCTusage() {
	if (!pt.src->HasRCT()){
		std::println("you arent able to use RCT");
		return;
	}
	std::println("1-Enable RCT, 2-Boost RCT, 3-Disable RCT");
	int choice = Utilities::GetInput<int>();
	switch (choice) {
	case 1:
		pt.src->EnableRCT();
		std::println("You have started using RCT");
		break;
	case 2:
		pt.src->BoostRCT();
		std::println("You have started pumping RCT at maximum output");
		break;
	case 3:
		pt.src->DisableRCT();
		std::println("You have disabled RCT");
		break;
	default:
		std::println("Invalid RCT Choice");
	}
}

void PlayerManager::PlayerDAusage() {
	std::println("1-On, 2-Off\n=>");
	int choice = Utilities::GetInput<int>();

	switch (choice) {
	case 1:
		pt.crs->SetAmplification(true);
		break;
	case 2:
		pt.crs->SetAmplification(false);
		break;
	}
}

void PlayerManager::PlayerShikigami() {
	if (pt.crs->GetShikigami().empty()) {
		std::println("You dont have any shikigami to use");
		return;
	}
	int count = 1;
	for (const auto& sh : pt.crs->GetShikigami()) {
		std::println("{}: {} ", count++, sh->GetName());
	}
	std::println("Choose the shikigami you'd like to use\n=> ");

	size_t ch = Utilities::GetInput<size_t>(); 
	if (ch > 0 && ch <= pt.crs->GetShikigami().size()) {
		ch--;
		Shikigami* sk = pt.crs->ChooseShikigami(ch);
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
		default:
			std::println("Invalid input.");
			break;
		}

	}
	else {
		std::println("Invalid Choice");
	}
}

void PlayerManager::PlayerReinforcement() {
	std::println("more reinforcement means harder hit to your CE spending");
	std::println("Current: {}", pt.crs->GetReinforcementStatus());
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
			pt.crs->AddReinforcement(c);
			break;
		case 2:
			pt.crs->AddReinforcement(-c);
			break;
		case 3:
			pt.crs->SetCurrentReinforcement(c);		
			break;
		}
	}

}

Character* PlayerManager::TargetSelector(Battlefield& bf) {
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
	if (!(std::cin >> t) || t >= bf.battlefield.size() || bf.battlefield[t].get() == pt.player || bf.battlefield[t]->GetCharacterHealth() <= 0.0) {
		if (std::cin.fail()) { 
			std::cin.clear(); 
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
		}
		std::println("Target missed or invalid!");
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
		return nullptr;
	}
	return bf.battlefield[t].get();
}