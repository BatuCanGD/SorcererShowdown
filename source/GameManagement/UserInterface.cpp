#include "code/header/GameManagement/UserInterface.h"
#include "code/header/Characters/Shikigami/Shikigami.h"
#include "code/header/Techniques/Techniques.h"
#include "code/header/CursedTools/CursedTool.h"
#include "code/header/Specials/Specials.h"
#include "code/header/Domains/Domain.h"
#include "code/header/Characters/CurseUsers/Sorcerers/Sorcerer.h"
#include "code/header/GameManagement/Utils.h"



void UserInterface::ShowBattleEntry(const std::vector<std::unique_ptr<Character>>& battlefield) {
	if (battlefield.size() == 2) {
		std::println("Its just you and {}. Defeat them and win", battlefield[1]->GetName());
	}
	else if (battlefield.size() > 9) {
		std::println("You, {}, and {} more sorcerers are in the area. Defeat all of them to win", battlefield[1]->GetName(), battlefield.size() - 2);
	}
	else {
		std::println("You and {} more sorcerers are near you. Defeat them all to win", battlefield.size() - 1);
	}

	std::println("-------Let the battle between {} sorcerers begin!-------", battlefield.size());
	std::println("-------------------------------------------------------");
}

void UserInterface::DisplaySorcererStatus(Character* s) {
	if (s->IsThePlayer()) {
		std::println("-------------Player's ({}'s) Turn-------------- {}", s->GetNameWithID(), s->IsCharacterStunned() ? "(Stunned)" : "");
	}
	else {
		std::println("-------------{}'s Turn-------------- {}", s->GetNameWithID(), s->IsCharacterStunned() ? "(Stunned)" : "");
	}
	double hp = s->GetCharacterHealth();
	double max_hp = s->GetCharacterMaxHealth();
	std::string hp_color = Utilities::Color::Green;

	if (!s->HPMoreThanMax(0.10)) {
		hp_color = Utilities::Color::Red;
	}
	else if (!s->HPMoreThanMax(0.33)) {
		hp_color = Utilities::Color::BrightRed;
	}
	else if (!s->HPMoreThanMax(0.66)) {
		hp_color = Utilities::Color::Yellow;
	}
	else {
		hp_color = Utilities::Color::Green;
	}
	

	std::print("Health [{}{:.1f}/{:.1f}{}] | ",hp_color, hp, max_hp, Utilities::Color::Clear);
	
	if (s->IsaCurseUser()) {
		auto crs = static_cast<CurseUser*>(s);
		double ce = crs->GetCharacterCE();
		double max_ce = crs->GetCharacterMaxCE();
		std::string ce_color = Utilities::Color::Cyan;

		if (!crs->CEMoreThanMax(0.10)) {
			ce_color = Utilities::Color::DimGray;
		}

		if (crs->GetTechnique() != nullptr) {
			std::print("Cursed Energy [{}{:.1f}/{:.1f}{}] | Technique [{}]:[{}]-[{}]\n",
			ce_color, ce, max_ce, Utilities::Color::Clear, 
			crs->GetTechnique()->GetTechniqueName(),
			crs->GetTechnique()->GetStringStatus(),
			crs->GetTechnique()->GetStringChantLevel());
		}else{
			std::println("Cursed Energy [{}{:.1f}/{:.1f}{}]", ce_color, ce, max_ce, Utilities::Color::Clear);
		}
		if (s->IsaSorcerer()) {
			auto sorcerer = static_cast<Sorcerer*>(s);
			std::print("Domain Amp [{}] | Reverse Cursed Technique [{}] | Reinforcement [{}]",
				sorcerer->GetDAstatus(), sorcerer->GetRCTstatus(), sorcerer->GetReinforcementStatus());
		}
		else if (s->IsaCurseUser()){
			std::print("Domain Amp [{}] | Reinforcement [{}]",
				crs->GetDAstatus(), crs->GetReinforcementStatus());
		}
	}

	std::println("");
	if (s->IsaCurseUser()) {
		auto sorcerer = static_cast<Sorcerer*>(s);
		if (sorcerer->GetDomain() != nullptr) {
			std::print("Domain [{}]:[{}] | ",
				sorcerer->GetDomain()->GetDomainName(),
				sorcerer->GetDomainStatus());

		}
		if (sorcerer->GetCounterDomain() != nullptr) {
			std::print("Counter [{}]:[{}] | ",
				sorcerer->GetCounterDomain()->GetDomainName(),
				sorcerer->GetCounterStatus());
		}
	}
	if (!s->GetCursedTools().empty() || s->GetTool() != nullptr) {
		std::print("Inventory ");
		if (s->GetCursedTools().empty()) {
			std::println("[{}Empty{}]",Utilities::Color::DimGray,Utilities::Color::Clear);
		}
		else {
			for (const auto& t : s->GetCursedTools()) {
				std::print("[{}] ", t->GetName());
			}
			std::println("");
		}
		std::print("Current Tool");

		if (s->GetTool() != nullptr) {
			std::println(" [{}]", s->GetTool()->GetName());
		}
		else {
			std::println(" [{}None{}]",Utilities::Color::DimGray,Utilities::Color::Clear);
		}
	}
	if (s->IsThePlayer() && s->IsCharacterStunned()) {
		std::println("\n\n");
		std::println("You have been Stunned and your turn has been skipped!");
		std::println("\n\n");
	}
	else if (s->IsThePlayer()) {
		std::println("\nChoose action:");

		std::string techniqued = std::format("1 - Technique [{}None{}] ", Utilities::Color::DimGray, Utilities::Color::Clear);
		std::string speciald = std::format("3 - Special [{}None{}] ", Utilities::Color::DimGray, Utilities::Color::Clear);
		std::string domaind = std::format("4 - Domain [{}Locked{}]", Utilities::Color::DimGray, Utilities::Color::Clear);
		std::string settingd = std::format("9 - Technique Settings [{}Locked{}]", Utilities::Color::DimGray, Utilities::Color::Clear);
		std::string rctd = std::format("6 - Reverse Cursed Technique [{}Locked{}]", Utilities::Color::DimGray, Utilities::Color::Clear);
		std::string amplificationd = std::format("7 - Domain Amplification [{}Locked{}]", Utilities::Color::DimGray, Utilities::Color::Clear);
		std::string shikigami = std::format("10 - Shikigami [{}None{}]", Utilities::Color::DimGray, Utilities::Color::Clear);

		if (s->IsaCurseUser()) {
			CurseUser* p_cuser = static_cast<CurseUser*>(s);
			Domain* domain = p_cuser->GetDomain();
			Domain* counter = p_cuser->GetCounterDomain();
			Technique* tech = p_cuser->GetTechnique();
			Specials* special = p_cuser->GetSpecial();
			

			techniqued = (tech == nullptr)
				? std::format("1 - Technique [{}None{}] ", Utilities::Color::DimGray, Utilities::Color::Clear)
				: std::format("1 - Technique [{}] ", tech->GetTechniqueName());

			speciald = (special == nullptr)
				? std::format("3 - Special [{}None{}] ", Utilities::Color::DimGray, Utilities::Color::Clear)
				: std::format("3 - Special [{}] ", special->GetSpecialName());

			domaind = (domain == nullptr && counter == nullptr)
				? std::format("4 - Domain [{}Locked{}]", Utilities::Color::DimGray, Utilities::Color::Clear)
				: "4 - Domain Actions";

			settingd = (tech == nullptr)
				? std::format("9 - Technique Settings [{}Locked{}]", Utilities::Color::DimGray, Utilities::Color::Clear)
				: "9 - Technique Settings";

			rctd = p_cuser->IsaSorcerer()
				? std::format("6 - Reverse Cursed Technique [{}]", static_cast<Sorcerer*>(p_cuser)->GetRCTstatus())
				: std::format("6 - Reverse Cursed Technique [{}Locked{}]", Utilities::Color::DimGray, Utilities::Color::Clear);

			amplificationd = p_cuser->IsPhysicallyGifted()
				? std::format("7 - Domain Amplification [{}Locked{}]", Utilities::Color::DimGray, Utilities::Color::Clear)
				: std::format("7 - Domain Amplification [{}]", p_cuser->GetDAstatus());

			shikigami = p_cuser->GetShikigami().empty()
				? std::format("10 - Shikigami [{}None{}]", Utilities::Color::DimGray, Utilities::Color::Clear)
				: "10 - Shikigami";
		}

		std::string fightd = "2 - Fight";
		std::string tauntd = "5 - Taunt";
		std::string toold = (s->GetCursedTools().empty() && s->GetTool() == nullptr)
			? std::format("8 - Cursed Tools [{}None{}]", Utilities::Color::DimGray, Utilities::Color::Clear)
			: "8 - Cursed Tools";
		std::string reinforcement = s->IsaSorcerer()
			? "11 - Reinforcement Level"
			: std::format("11 - Reinforcement [{}Locked{}]", Utilities::Color::DimGray, Utilities::Color::Clear);

		size_t col_width = 35;
		std::println("  {} | {} | {}",
			Utilities::StringPad(techniqued, col_width),
			Utilities::StringPad(fightd, col_width),
			Utilities::StringPad(speciald, col_width));

		std::println("  {} | {} | {}",
			Utilities::StringPad(domaind, col_width),
			Utilities::StringPad(tauntd, col_width),
			Utilities::StringPad(rctd, col_width));

		std::println("  {} | {} | {}",
			Utilities::StringPad(amplificationd, col_width),
			Utilities::StringPad(toold, col_width),
			Utilities::StringPad(settingd, col_width));

		std::println("  {} | {}",
			Utilities::StringPad(shikigami, col_width),
			Utilities::StringPad(reinforcement, col_width));

		std::print("\n=> ");
	}
}
void UserInterface::ContinuePrompt(bool end_of_round = false) {
	if (end_of_round) {
		std::println("Press Enter to begin the next round...");
	} else {
		std::println("Press Enter to continue...");
	}
	Utilities::WaitForInput();
}

void UserInterface::ClearScreen() {
	std::print("\033[2J\033[H");
}