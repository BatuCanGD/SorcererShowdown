#include "GameManagement/PlayerManager.h"
#include "Characters/CurseUsers/Sorcerers/Sorcerer.h"
#include "Characters/Shikigami/Shikigami.h"

#include "BindingVows/BindingVowList.h"
#include "Techniques/Techniques.h"
#include "CursedTools/CursedTool.h"
#include "Specials/Specials.h"
#include "Domains/Domain.h"

#include "GameManagement/BattlefieldHeader.h"
#include "GameManagement/UserInterface.h"
#include "GameManagement/Colors.h"
#include "GameManagement/Utils.h"
#include "GameManagement/VList.h"

std::vector<PlayerManager::Action> PlayerManager::DealWithActions(Character* s, CurseUser* crs, Sorcerer* src){
	std::println("\nChoose action:");
	std::vector<Action> acts;
	auto add_choice = [&] (Action act, std::string_view label) {
		acts.push_back(act);
		std::println("{} - {}", acts.size(), label);
	};

	add_choice(Action::Attack, "Attack");
	if (s->GetTool() || !s->GetCursedTools().empty()){
		add_choice(Action::CursedTools, "Cursed Tools");
	}
	add_choice(Action::Taunt, "Taunt");
	if (crs){
		if (auto* tech = crs->GetTechnique()){
			add_choice(Action::Technique, std::format("Technique [{}]", tech->GetTechniqueName()));
			add_choice(Action::TechSettings, "Technique Settings");
		}
		if (crs->GetDomain() || crs->GetCounter()){
			add_choice(Action::Domain, "Domain Actions");
		}
		add_choice(Action::Reinforcement, "CE Reinforcement");
		add_choice(Action::BindingVows, "Binding Vows");
		if (!crs->GetShikigami().empty()){
			add_choice(Action::Shikigami, "Shikigami");
		}
		if (auto* spec = crs->GetSpecial()){
			add_choice(Action::Special, std::format("Special [{}]", spec->GetSpecialName()));
		}
		if (src && src->HasRCT()){
			add_choice(Action::RCT, std::format("Reverse Cursed Technique [{}]", src->GetRCTstatus()));
		}
	}
	return acts;
}

void PlayerManager::OnPlayerTurn(Character* player, Battlefield& bf) {
	if (player->IsCharacterStunned()){
		std::println("\n\nYou have been Stunned and your turn has been skipped!\n\n");
		return;
	}

	CurseUser* crs = player->IsaCurseUser() ? static_cast<CurseUser*>(player) : nullptr;
	Sorcerer* src = (crs && crs->IsaSorcerer()) ? static_cast<Sorcerer*>(crs) : nullptr;

	while (true) {
		const auto choices = DealWithActions(player, crs, src);

		size_t input = Utilities::GetInput<size_t>();
		while (input < 1 || input > choices.size()) {
			std::println("Invalid Input!");
			input = Utilities::GetInput<size_t>();
		}

		bool success = false;
		switch (choices[input - 1]) {
		case Action::Attack:
			success = DealWithFighting(player, bf);
			break;
		case Action::Taunt:
			success = DealWithTaunting(player, bf);
			break;
		case Action::Technique:
			success = DealWithTechnique(crs, bf);
			break;
		case Action::Special:
			success = DealWithSpecial(crs, bf);
			break;
		case Action::Domain:
			success = DealWithDomain(crs);
			break;
		case Action::RCT:
			success = DealWithRCT(src);
			break;
		case Action::CursedTools:
			success = DealWithCursedTools(player);
			break;
		case Action::TechSettings:
			success = DealWithTechSettings(crs, bf);
			break;
		case Action::Shikigami:
			success = DealWithShikigami(crs);
			break;
		case Action::Reinforcement:
			success = DealWithReinforcement(crs);
			break;
		case Action::BindingVows:
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
	std::println("1 - Set reinforcement amount | 2 - return");
	std::print("=> ");

	const int choice = Utilities::GetInput<int>();
	if (choice == 2){ 
		return false;
	}else if (choice != 1) {
		std::println("Invalid Input!");
		return false;
	}

	std::println("\nWrite out the amount that you would like to set the reinforcement to\n=> ");
	c->SetCurrentReinforcement(Utilities::GetInput<double>());
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
		std::println("You dont have a domain and a counter");
		return false;
	}

	if (domain) {
		std::println("1 - {} Status: [{}]", domain->GetDomainName(), domain->GetDomainStatus());
	}
	if (counter) {
		std::println("2 - {} Status: [{}]", counter->GetDomainName(), counter->GetDomainStatus());
	}
	std::println("3 - Domain Amplification: [{}]", c->GetDAstatus());

	std::print("=> ");
	int dch = Utilities::GetInput<int>();

	if (dch < 1 || dch > 3){
		std::println("Invalid Input");
		return false;
	}

	std::println("1 - Activate | 2 - Deactivate\n==>>");
	int ch = Utilities::GetInput<int>();

	switch (dch) {
	case 1:
		if (!domain) {
			std::println("You dont have a domain");
			return false;
		}
		domain->SetDomainActivation(c, ch == 1);
		return true;
	case 2:
		if (!counter) {
			std::println("You dont have a counter");
			return false;
		}
		counter->SetDomainActivation(c, ch == 1);
		return true;
	case 3:
		c->SetAmplification(ch == 1);
		break;
	default:
		std::println("Invalid Input");
	}
	return false;
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