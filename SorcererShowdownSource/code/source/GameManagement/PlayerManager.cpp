#include "GameManagement/PlayerManager.h"
#include "BindingVows/BindingVowList.h"
#include "GameManagement/BattlefieldHeader.h"
#include "GameManagement/UserInterface.h"
#include "Characters/Shikigami/Shikigami.h"
#include "Techniques/Techniques.h"
#include "CursedTools/CursedTool.h"
#include "Specials/Specials.h"
#include "Domains/Domain.h"
#include "Characters/CurseUsers/Sorcerers/Sorcerer.h"
#include "GameManagement/Colors.h"
#include "GameManagement/Utils.h"
#include "GameManagement/VList.h"


void PlayerManager::OnPlayerTurn(Character* player, Battlefield& bf) {
	CurseUser* crs = player->IsaCurseUser() ? static_cast<CurseUser*>(player) : nullptr;
	Sorcerer* src = (crs && crs->IsaSorcerer()) ? static_cast<Sorcerer*>(crs) : nullptr;

	while (true) {
		UserInterface::GetPlayerActions(player);
		if (player->IsCharacterStunned()) break;

		int act = Utilities::GetInput<int>();
		while (act < 1 || act > 12) {
			std::println("Invalid Input!");
			act = Utilities::GetInput<int>();
		}

		bool success = false;
		switch (act) {
		case 1:
			success = DealWithTechnique(crs, bf);
			break;
		case 2:
			success = DealWithFighting(player, bf);
			break;
		case 3:
			success = DealWithSpecial(crs, bf);
			break;
		case 4:
			success = DealWithDomain(crs);
			break;
		case 5:
			success = DealWithTaunting(player, bf);
			break;
		case 6:
			success = DealWithRCT(src);
			break;
		case 7:
			success = DealWithDomainAmp(crs);
			break;
		case 8:
			success = DealWithCursedTools(player);
			break;
		case 9:
			success = DealWithTechSettings(crs, bf);
			break;
		case 10:
			success = DealWithShikigami(crs);
			break;
		case 11:
			success = DealWithReinforcement(crs);
			break;
		case 12:
			success = DealWithBindingVows(crs);
			break;
		}
		if (success) break;
	}
}


bool PlayerManager::DealWithCursedTools(Character* c) {
	if (!c->GetTool() && c->GetCursedTools().empty()) {
		std::println("You dont have any cursed tools!");
		return false;
	}
	std::println("Would you like to change your current tool\n1 - Yes | 2 - No\n=> ");
	if (Utilities::GetInput<int>() != 1) return false;

	std::println("Available Tools:");
	int count = 1;
	for (const auto& tool : c->GetCursedTools()) {
		std::println("{} - {}", count++, tool->GetName());
	}

	const bool has_equipped_tool = c->GetTool() != nullptr;
	if (has_equipped_tool) {
		std::println("\n0 - Unequip ({})", c->GetTool()->GetName());
	}
	else {
		std::println("\n0 - Go Back");
	}

	std::print("\n=> ");
	const size_t choice = Utilities::GetInput<size_t>();
	if (choice > c->GetCursedTools().size()) {
		std::println("Invalid Choice");
		return false;
	}
	if (choice == 0 && !has_equipped_tool) return false;

	c->CursedToolChoice(choice);
	return true;
}

bool PlayerManager::DealWithFighting(Character* c, Battlefield& bf) {
	std::println("Would you like to engage in Close Combat?\n1 - Yes | 2 - No\n=> ");
	if (Utilities::GetInput<int>() != 1) return false;

	CurseUser* crs = c->IsaCurseUser() ? static_cast<CurseUser*>(c) : nullptr;
	Character* target = VList::TargetSelector(bf, crs);

	std::println("{} engages in close combat with {}!", c->GetNameWithID(), target->GetNameWithID());
	c->Attack(target);
	return true;
}

bool PlayerManager::DealWithTaunting(Character* c, Battlefield& bf) {
	std::println("Would you like to Taunt another Opponent?\n1 - Yes | 2 - No\n=> ");
	if (Utilities::GetInput<int>() != 1) return false;
	CurseUser* crs = c->IsaCurseUser() ? static_cast<CurseUser*>(c) : nullptr;
	c->Taunt(VList::TargetSelector(bf, crs));
	return true;
}

bool PlayerManager::DealWithDomainAmp(CurseUser* c) {
	if (!c) {
		std::println("You cant use Domain Amplification.");
		return false;
	}

	std::println("1 - On | 2 - Off | 3 - Return\n=>");
	switch (Utilities::GetInput<int>()) {
	case 1:
		c->SetAmplification(true);
		break;
	case 2:
		c->SetAmplification(false);
		break;
	default:
		std::println("Invalid input.");
	}
	return false;
}

bool PlayerManager::DealWithBindingVows(CurseUser* c) {
	if (!c) {
		std::println("You cant use Binding Vows");
		return false;
	}

	auto& player_vows = c->GetBindingVows();
	const auto& binding_vows = BindingVow::GetBindingVows();
	std::vector<BindingVow*> vow_list;
	int number = 0;

	std::println("\n\n****{}Active Binding Vows{}****: ", Color::Green, Color::Clear);
	for (auto& vow : player_vows) {
		std::println("{}: {}", ++number, vow->GetVowDetails());
		vow_list.push_back(vow.get());
	}

	std::println("\n\n****{}Available Binding Vows{}****: ", Color::Yellow, Color::Clear);
	for (auto& available_vow : binding_vows) {
		bool already_active = false;
		for (auto& active_vow : player_vows) {
			if (active_vow->GetVowDetails() == available_vow->GetVowDetails()) {
				already_active = true;
				break;
			}
		}

		if (already_active) continue;
		std::println("{}: {}", ++number, available_vow->GetVowDetails());
		vow_list.push_back(available_vow.get());
	}
	std::print("=> ");

	const size_t choice = Utilities::GetInput<size_t>();
	if (choice == 0 || choice > vow_list.size()) {
		std::println("Invalid choice.");
		return false;
	}

	BindingVow* chosen = vow_list[choice - 1];
	for (size_t i = 0; i < player_vows.size(); ++i) {
		if (player_vows[i].get() != chosen) continue;

		std::println("1 - Remove Binding Vow | 0 - Cancel");
		if (Utilities::GetInput<int>() != 1) return false;

		c->RemoveBindingVow(i);
		std::println("Binding Vow removed.");
		return true;
	}

	std::println("1 - Use Binding Vow | 0 - Cancel");
	if (Utilities::GetInput<int>() != 1) return false;

	c->AddBindingVow(chosen->Clone());
	std::println("Binding Vow added!");
	return true;
}

bool PlayerManager::DealWithReinforcement(CurseUser* c) {
	if (!c) {
		std::println("You cant use Cursed Energy Reinforcement");
		return false;
	}

	std::println("More reinforcement means a harder hit to your CE spending");
	std::println("Current: {}", c->GetReinforcementStatus());
	std::println("1 - Add reinforcement amount | 2 - Subtract reinforcement amount | 3 - Set reinforcement amount");
	std::print("=> ");

	const int choice = Utilities::GetInput<int>();
	if (choice < 1 || choice > 3) {
		std::println("Invalid input.");
		return false;
	}

	switch (choice) {
	case 1:
		std::println("\nWrite out the amount you would like to reinforce by");
		break;
	case 2:
		std::println("\nWrite out the amount you would like to reduce reinforcement by");
		break;
	case 3:
		std::println("\nWrite out the amount that you would like to set the reinforcement to");
		break;
	}

	std::print("=> ");
	const double amount = Utilities::GetInput<double>();
	switch (choice) {
	case 1:
		c->AddReinforcement(amount);
		break;
	case 2:
		c->AddReinforcement(-amount);
		break;
	case 3:
		c->SetCurrentReinforcement(amount);
		break;
	}
	return true;
}

bool PlayerManager::DealWithTechnique(CurseUser* c, Battlefield& bf) {
	if (!c) {
		std::println("You cant use techniques!");
		return false;
	}
	if (!c->GetTechnique()) {
		std::println("You don't have a technique to use!");
		return false;
	}
	if (c->GetTechnique()->BurntOut()) {
		std::println("Your technique is burnt out, you cannot use it properly yet");
		return false;
	}
	return c->GetTechnique()->TechniqueMenu(c, VList::TargetSelector(bf, c), bf);
}

bool PlayerManager::DealWithDomain(CurseUser* c) {
	if (!c) {
		std::println("You cant use domains");
		return false;
	}

	Domain* domain = c->GetDomain();
	Domain* counter = c->GetCounter();
	if (!domain && !counter) {
		std::println("You dont have a domain or a counter to a domain");
		return false;
	}

	if (domain) {
		std::println("Domain Status: [{}]", domain->GetDomainStatus());
		std::print("1 - Activate Domain | 2 - Disable Domain ");
	}
	if (counter) {
		std::println("{} Status: [{}]", counter->GetDomainName(), counter->GetDomainStatus());
		std::println("\n3 - Activate {} | 4 - Disable {} ", counter->GetDomainName(), counter->GetDomainName());
	}

	std::print("=> ");
	switch (Utilities::GetInput<int>()) {
	case 1:
		if (!domain) {
			std::println("You dont have a domain");
			return false;
		}
		domain->SetDomainActivation(c, true);
		return true;
	case 2:
		if (!domain) {
			std::println("You dont have a domain");
			return false;
		}
		domain->SetDomainActivation(c, false);
		return true;
	case 3:
		if (!counter) {
			std::println("You dont have a counter");
			return false;
		}
		counter->SetDomainActivation(c, true);
		return true;
	case 4:
		if (!counter) {
			std::println("You dont have a counter");
			return false;
		}
		counter->SetDomainActivation(c, false);
		return true;
	default:
		std::println("Invalid Input");
		return false;
	}
}

bool PlayerManager::DealWithShikigami(CurseUser* c) {
	if (!c) {
		std::println("You cant use Shikigami");
		return false;
	}
	if (c->GetShikigami().empty()) {
		std::println("You dont have any shikigami to use");
		return false;
	}

	int count = 1;
	for (const auto& shikigami : c->GetShikigami()) {
		std::println("{}: {} ", count++, shikigami->GetName());
	}
	std::println("Choose the shikigami you'd like to use\n=> ");

	size_t choice = Utilities::GetInput<size_t>();
	if (choice == 0 || choice > c->GetShikigami().size()) {
		std::println("Invalid Choice");
		return false;
	}

	Shikigami* selected = c->ChooseShikigami(choice - 1);
	std::println("Chosen Shikigami: {} | [{}]", selected->GetName(), selected->GetShikigamiStatus());

	if (!selected->IsActive()) {
		std::println("1 - Manifest");
	}
	else {
		std::println("2 - Dismiss");
	}

	switch (Utilities::GetInput<int>()) {
	case 1:
		if (selected->IsActive()) {
			std::println("{} is already active!", selected->GetName());
			return false;
		}
		selected->Manifest();
		std::println("{} has been Summoned to aid you on the battlefield!", selected->GetName());
		return true;
	case 2:
		if (!selected->IsActive()) {
			std::println("{} is already dormant.", selected->GetName());
			return false;
		}
		selected->Withdraw();
		std::println("{} has been dismissed and is now dormant within the shadows.", selected->GetName());
		return true;
	default:
		std::println("Invalid input.");
		return false;
	}
}

bool PlayerManager::DealWithSpecial(CurseUser* c, Battlefield& bf) {
	if (!c) {
		std::println("You cant use Special Moves");
		return false;
	}
	if (!c->GetSpecial()) {
		std::println("You dont have a Special move to use");
		return false;
	}
	return c->GetSpecial()->UseSpecial(c, nullptr, bf);
}

bool PlayerManager::DealWithTechSettings(CurseUser* c, Battlefield& bf) {
	if (!c) {
		std::println("You cant use Techniques");
		return false;
	}
	if (!c->GetTechnique()) {
		std::println("You dont have a Technique");
		return false;
	}
	return c->GetTechnique()->TechniqueSetting(c, bf);
}

bool PlayerManager::DealWithRCT(Sorcerer* c) {
	if (!c || !c->HasRCT()) {
		std::println("You cant use Reverse Cursed Technique!");
		return false;
	}
	if (c->GetCharacterMaxCE() < 500.0) {
		std::println("You dont have enough cursed energy to continuously maintain Reverse Cursed Technique");
		return false;
	}

	std::println("1 - Set Reverse Cursed Technique Output | 2 - Return");
	if (Utilities::GetInput<int>() != 1) return false;

	std::println("Type the amount of RCT Output you would like to use to heal your wounds");
	c->SetRCTAmount(Utilities::GetInput<double>());
	return true;
}