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
#include "code/header/GameManagement/VList.h"

void PlayerManager::OnPlayerTurn(Character* player, Battlefield& bf) {
	CurseUser* crs = player->IsaCurseUser() ? static_cast<CurseUser*>(player) : nullptr;
	Sorcerer* src = (crs && crs->IsaSorcerer()) ? static_cast<Sorcerer*>(crs) : nullptr;

	int player_choice = Utilities::GetInput<int>();
	while (player_choice < 1 || player_choice > 12){
		std::println("Invalid Input!");
		player_choice = Utilities::GetInput<int>();
	}
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
		Character* target = VList::TargetSelector(bf, crs);
		if (target) {
			crs->GetTechnique()->TechniqueMenu(crs, target, bf);
		}
		break;
	}
	case 2: {
		if (Character* target = VList::TargetSelector(bf, crs)) {
			std::println("{} engages in close combat with {}!", player->GetNameWithID(), target->GetNameWithID());
			player->Attack(target);
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
		crs->GetSpecial()->UseSpecial(crs, nullptr, bf);
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
		if (Character* target = VList::TargetSelector(bf, crs)) {
			player->Taunt(target);
		}
		break;
	}
	case 6: {
		if (!src || !src->HasRCT()) {
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
		GetPlayerTools(player);
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

void PlayerManager::PlayerVows(CurseUser* crs) {
    auto& player_vows = crs->GetBindingVows();
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
            crs->RemoveBindingVow(inv);
            std::println("Binding Vow removed.");
        }
    } else {
        std::println("1 - Use Binding Vow | 0 - Cancel");
        int c = Utilities::GetInput<int>();
        if (c == 1) {
            crs->AddBindingVow(chosen->Clone());
            std::println("Binding Vow added!");
        }
    }
}

void PlayerManager::PlayerDomainUsage(CurseUser* crs) {
	if (!crs->GetDomain() && !crs->GetCounter()) {
		std::println("You dont have a domain and a counter to a domain");
		return;
	}
	if (auto* domain = crs->GetDomain()) {
		std::println("Domain Status: [{}]", domain->GetDomainStatus());
	}
	if (auto* ctr = crs->GetCounter()) {
		std::println("{} Status: [{}]", crs->GetCounter()->GetDomainName(), ctr->GetDomainStatus());
	}
	if (crs->GetDomain()) {
		std::print("1 - Activate Domain | 2 - Disable Domain ");
	}
	if (crs->GetCounter()) {
		std::println("\n3 - Activate {} | 4 - Disable {} ", crs->GetCounter()->GetDomainName(), crs->GetCounter()->GetDomainName());
	}
	std::print("=> ");
	int ch = Utilities::GetInput<int>();
	switch (ch) {
	case 1:
		if (!crs->GetDomain()) {
			std::println("You dont have a domain");
			break;
		}
		crs->GetDomain()->SetDomainActivation(crs, true);
		break;
	case 2:
		if (!crs->GetDomain()) {
			std::println("You dont have a domain");
			break;
		}
		crs->GetDomain()->SetDomainActivation(crs, false);
		break;
	case 3:
		if (!crs->GetCounter()) {
			std::println("You dont have a counter domain");
			break;
		}
		crs->GetCounter()->SetDomainActivation(crs, true);
		break;
	case 4:
		if (!crs->GetCounter()) {
			std::println("You dont have a counter domain");
			break;
		}
		crs->GetCounter()->SetDomainActivation(crs, false);
		break;
	default:
		std::println("Invalid Input");
	}
}

void PlayerManager::GetPlayerTools(Character* player) {
	int count = 1;
	std::println("Available Tools:");
	for (const auto& tool : player->GetCursedTools()) {
		std::println("{} - {}", count++, tool->GetName());
	}

	if (player->GetTool()) {
		std::println("\n0 - Unequip ({})", player->GetTool()->GetName());
	}
	else {
		std::println("\n0 - Go Back");
	}

	std::print("\n=> ");
	size_t choice = Utilities::GetInput<size_t>();
	player->CursedToolChoice(choice);
}

void PlayerManager::PlayerRCTusage(Sorcerer* src) {
	std::println("1-Enable RCT, 2-Boost RCT, 3-Disable RCT");
	int choice = Utilities::GetInput<int>();
	switch (choice) {
	case 1:
		src->EnableRCT();
		std::println("You have started using RCT");
		break;
	case 2:
		src->BoostRCT();
		std::println("You have started pumping RCT at maximum output");
		break;
	case 3:
		src->DisableRCT();
		std::println("You have disabled RCT");
		break;
	default:
		std::println("Invalid RCT Choice");
	}
}

void PlayerManager::PlayerDAusage(CurseUser* crs) {
	std::println("1-On, 2-Off\n=>");
	int choice = Utilities::GetInput<int>();

	switch (choice) {
	case 1:
		crs->SetAmplification(true);
		break;
	case 2:
		crs->SetAmplification(false);
		break;
	}
}

void PlayerManager::PlayerShikigami(CurseUser* crs) {
	if (crs->GetShikigami().empty()) {
		std::println("You dont have any shikigami to use");
		return;
	}
	int count = 1;
	for (const auto& sh : crs->GetShikigami()) {
		std::println("{}: {} ", count++, sh->GetName());
	}
	std::println("Choose the shikigami you'd like to use\n=> ");

	size_t ch = Utilities::GetInput<size_t>();
	if (ch > 0 && ch <= crs->GetShikigami().size()) {
		ch--;
		Shikigami* sk = crs->ChooseShikigami(ch);
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

void PlayerManager::PlayerReinforcement(CurseUser* crs) {
	std::println("more reinforcement means harder hit to your CE spending");
	std::println("Current: {}", crs->GetReinforcementStatus());
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
			crs->AddReinforcement(c);
			break;
		case 2:
			crs->AddReinforcement(-c);
			break;
		case 3:
			crs->SetCurrentReinforcement(c);
			break;
		}
	}
}
