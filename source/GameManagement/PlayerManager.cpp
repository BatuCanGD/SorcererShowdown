#include "code/header/GameManagement/PlayerManager.h"
#include "code/header/GameManagement/BattlefieldHeader.h"
#include "code/header/Characters/Shikigami/Shikigami.h"
#include "code/header/Techniques/Techniques.h"
#include "code/header/CursedTools/CursedTool.h"
#include "code/header/Specials/Specials.h"
#include "code/header/Domains/Domain.h"
#include "code/header/Characters/CurseUsers/Sorcerers/Sorcerer.h"
#include "code/header/GameManagement/Utils.h"



void PlayerManager::OnPlayerTurn(Character& s, Battlefield& bf) {
	int plrch = Utilities::GetValidInput();
	bool p_curseuser = s.IsaCurseUser();
	bool p_sorcerer = s.IsaSorcerer();

	switch (plrch) {
	case 1: {
		if (p_curseuser) {
			auto crs = static_cast<CurseUser*>(&s);
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
		auto cuser = static_cast<CurseUser*>(&s);
		Character* target = TargetSelector(bf, &s);
		if (target) {
			cuser->GetTechnique()->TechniqueMenu(cuser, target, bf);
		}
		break;
	}
	case 2: {
		if (Character* target = TargetSelector(bf, &s)) {
			std::println("{} engages in close combat with {}!", s.GetName(), target->GetName());
			s.Attack(target);
		}
		break;
	}
	case 3: {
		if (p_curseuser) {
			auto crs = static_cast<CurseUser*>(&s);
			if (crs->GetSpecial() == nullptr) {
				std::println("You dont have a Special move to use");
				break;
			}
		}
		else {
			std::println("You cant Special Moves");
			break;
		}
		auto cuser = static_cast<CurseUser*>(&s);

		cuser->GetSpecial()->PerformSpecial(cuser);
		break;
	}
	case 4: {
		if (!p_curseuser) {
			std::println("You cant use domains");
			break;
		}
		PlayerDomainUsage(s);
		break;
	}
	case 5: {
		if (Character* target = TargetSelector(bf, &s)) {
			s.Taunt(target);
		}
		break;
	}
	case 6: {
		if (!p_sorcerer) {
			std::println("You cant use Reverse Cursed Technique!");
			break;
		}
		auto src = static_cast<Sorcerer*>(&s);
		if (src->GetCharacterMaxCE() < 500.0) {
			std::println("You dont have enough cursed energy to continuously maintain Reverse Cursed Technique");
			break;
		}
		PlayerRCTusage(s);
		break;
	}
	case 7:
		if (!p_curseuser) {
			std::println("You cant use Domain Amplification.");
			break;
		}
		PlayerDAusage(s);
		break;
	case 8:
		GetPlayerTools(s);
		break;
	case 9:
		if (p_curseuser) {
			auto src = static_cast<CurseUser*>(&s);
			if (src->GetTechnique()) {
				src->GetTechnique()->TechniqueSetting(src, bf);
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
		if (!p_curseuser) {
			std::println("You cant use Shikigami");
			return;
		}
		PlayerShikigami(s);
		break;
	case 11:
		if (!p_curseuser) {
			std::println("You cant use Cursed Energy Reinforcement");
			return;
		}
		PlayerReinforcement(s);
		break;
	default:
		std::println("Invalid Choice");
	}
}

void PlayerManager::PlayerDomainUsage(Character& s) {
	auto p_sorcerer = static_cast<CurseUser*>(&s);

	if (p_sorcerer->GetDomain() == nullptr && p_sorcerer->GetCounterDomain() == nullptr) {
		std::println("You dont have a domain and a counter to a domain");
		return;
	}
	if (p_sorcerer->GetDomain() != nullptr) {
		std::println("Domain Status: [{}]", p_sorcerer->DomainActive() ? "Active" : "Inactive");
	}
	if (p_sorcerer->GetCounterDomain() != nullptr) {
		std::println("{} Status: [{}]", p_sorcerer->GetCounterDomain()->GetDomainName(), p_sorcerer->CounterDomainActive() ? "Active" : "Inactive");
	}
	if (p_sorcerer->GetDomain() != nullptr) {
		std::print("1 - Activate Domain | 2 - Disable Domain ");
	}
	if (p_sorcerer->GetCounterDomain() != nullptr) {
		std::println("\n3 - Activate {} | 4 - Disable {} ", p_sorcerer->GetCounterDomain()->GetDomainName(), p_sorcerer->GetCounterDomain()->GetDomainName());
	}
	std::print("=> ");
	int ch = Utilities::GetValidInput();
	switch (ch) {
	case 1:
		if (p_sorcerer->GetDomain() == nullptr) {
			std::println("You dont have a domain");
			break;
		}
		p_sorcerer->ActivateDomain();
		break;
	case 2:
		if (p_sorcerer->GetDomain() == nullptr) {
			std::println("You dont have a domain");
			break;
		}
		p_sorcerer->DeactivateDomain();
		break;
	case 3:
		if (p_sorcerer->GetCounterDomain() == nullptr) {
			std::println("You dont have a counter domain");
			break;
		}
		p_sorcerer->ActivateCounterDomain();
		break;
	case 4:
		if (p_sorcerer->GetCounterDomain() == nullptr) {
			std::println("You dont have a counter domain");
			break;
		}
		p_sorcerer->DeactivateCounterDomain();
		break;
	default:
		std::println("Invalid Input");
	}
}

void PlayerManager::GetPlayerTools(Character& s) {
	int count = 1;
	std::println("Available Tools:");
	for (const auto& tool : s.GetCursedTools()) {
		std::println("{} - {}", count++, tool->GetName());
	}

	if (s.GetTool() != nullptr) {
		std::println("\n0 - Unequip ({})", s.GetTool()->GetName());
	}
	else {
		std::println("\n0 - Go Back");
	}

	std::print("\n=> ");
	int choice = Utilities::GetValidInput();
	s.CursedToolChoice(static_cast<size_t>(choice));
}

void PlayerManager::PlayerRCTusage(Character& s) {
	auto p_sorcerer = static_cast<Sorcerer*>(&s);

	std::println("1-Enable RCT, 2-Boost RCT, 3-Disable RCT");
	int choice = Utilities::GetValidInput();
	switch (choice) {
	case 1:
		p_sorcerer->EnableRCT();
		std::println("You have started using RCT");
		break;
	case 2:
		p_sorcerer->BoostRCT();
		std::println("You have started pumping RCT at maximum output");
		break;
	case 3:
		p_sorcerer->DisableRCT();
		std::println("You have disabled RCT");
		break;
	default:
		std::println("Invalid RCT Choice");
	}
}

void PlayerManager::PlayerDAusage(Character& s) {
	auto p_cuser = static_cast<CurseUser*>(&s);
	std::println("1-On, 2-Off\n=>");
	int choice = Utilities::GetValidInput();

	switch (choice) {
	case 1:
		p_cuser->SetAmplification(true);
		break;
	case 2:
		p_cuser->SetAmplification(false);
		break;
	}
}

void PlayerManager::PlayerShikigami(Character& s) {
	auto p = static_cast<Sorcerer*>(&s);

	if (p->GetShikigami().empty()) {
		std::println("You dont have any shikigami to use");
		return;
	}
	int count = 1;
	for (const auto& sh : p->GetShikigami()) {
		std::println("{}: {} ", count, sh->GetName());
		count++;
	}
	std::println("Choose the shikigami you'd like to use\n=> ");

	size_t ch = static_cast<size_t>(Utilities::GetValidInput()); 
	if (ch > 0 && ch <= p->GetShikigami().size()) {
		ch--;
		Shikigami* sk = p->ChooseShikigami(ch);
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

		int cs = Utilities::GetValidInput();
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

void PlayerManager::PlayerReinforcement(Character& s) {
	auto p = static_cast<Sorcerer*>(&s);
	std::println("more reinforcement means harder hit to your CE spending");
	std::println("Current: {}", p->GetReinforcementStatus());
	std::println("1 - Add reinforcement amount 2 - Subtract reinforcement amount  3 - Set reinforcement amount");
	std::print("=> "); int ch = Utilities::GetValidInput();
	if (ch == 1) {
		std::println("\nWrite out the amount you would like to reinforce by");
		std::print("=> "); double vl = Utilities::GetPreciseInput();
		p->AddReinforcement(vl);
	}
	else if (ch == 2) {
		std::println("\nWrite out the amount you would like reduce reinforcement by");
		std::print("=> "); double vl = -Utilities::GetPreciseInput();
		p->AddReinforcement(vl);
	}
	else if (ch == 3) {
		std::println("\nWrite out the amount that you would like to set the reinforcement to");
		std::print("=> "); double vl = Utilities::GetPreciseInput();
		p->SetCurrentReinforcement(vl);
	}
	else {
		std::println("Invalid Input.");
	}
}

Character* PlayerManager::TargetSelector(Battlefield& bf, Character* player) {
	std::println("Choose your target:");
	for (size_t i = 0; i < bf.battlefield.size(); ++i) {
		auto& current = *bf.battlefield[i];
		if (current.GetCharacterHealth() <= 0) continue;

		double health = current.GetCharacterHealth();
		double cursed_energy = 0;
		Technique* tech = nullptr;
		Domain* domain = nullptr;

		if (current.IsaCurseUser()) {
			auto* crs = static_cast<CurseUser*>(&current);
			cursed_energy = crs->GetCharacterCE();
			tech = crs->GetTechnique();
			domain = crs->GetDomain();
		}

		std::string t_status = (tech == nullptr) ? "" : std::format("| Technique status: [{}] ", tech->GetStringStatus());
		std::string d_status = (domain == nullptr) ? "" : std::format("| Domain status: [{}] ", static_cast<CurseUser*>(&current)->GetDomainStatus());

		std::string stunned = current.IsCharacterStunned() ? " (Stunned)" : "";
		std::string name = current.GetName();
		std::string ce_display = current.IsPhysicallyGifted() ? "Heavenly Restricted" : std::format("{:.1f} CE", cursed_energy);

		if (bf.battlefield[i].get() == player) {
			std::println("{}: {} (You)",
							i, name);
		}
		else {
			std::println("{}: {}{} | ({:.1f} HP) ({}) {}{} ",
						i, name, stunned, health, ce_display, t_status, d_status);
		}
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