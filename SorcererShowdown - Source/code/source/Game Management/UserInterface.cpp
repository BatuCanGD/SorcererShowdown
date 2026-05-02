#include "UIDisplay.h"
#include "Shikigami.h"
#include "Techniques.h"
#include "CursedTool.h"
#include "Specials.h"
#include "Domain.h"
#include "Sorcerer.h"
#include "Utils.h"



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
	std::string hp_color = Color::Green;

	if (!s->HPMoreThanMax(0.10)) {
		hp_color = Color::Red;
	}
	else if (!s->HPMoreThanMax(0.33)) {
		hp_color = Color::BrightRed;
	}
	else if (!s->HPMoreThanMax(0.66)) {
		hp_color = Color::Yellow;
	}
	else {
		hp_color = Color::Green;
	}
	

	std::print("Health: [{}{:.1f}/{:.1f}{}] | ",hp_color, hp, max_hp, Color::Clear);
	
	if (s->IsaCurseUser()) {
		auto crs = static_cast<CurseUser*>(s);
		double ce = crs->GetCharacterCE();
		double max_ce = crs->GetCharacterMaxCE();
		std::string ce_color = Color::Cyan;

		if (!crs->CEMoreThanMax(0.10)) {
			ce_color = Color::DimGray;
		}

		std::println("Cursed Energy: [{}{:.1f}/{:.1f}{}]",
			ce_color, ce, max_ce, Color::Clear);
		if (s->IsaSorcerer()) {
			auto sorcerer = static_cast<Sorcerer*>(s);
			std::print("Domain Amplification: [{}] | Reverse Cursed Technique: [{}] | CE Reinforcement: [{}]",
				sorcerer->GetDAstatus(), sorcerer->GetRCTstatus(), sorcerer->GetReinforcementStatus());
		}
		else if (s->IsaCurseUser()){
			auto crs = static_cast<CurseUser*>(s);
			std::print("Domain Amplification: [{}] | CE Reinforcement: [{}]",
				crs->GetDAstatus(), crs->GetReinforcementStatus());
		}
	}

	std::println("");
	if (s->IsaCurseUser()) {
		auto sorcerer = static_cast<Sorcerer*>(s);
		if (sorcerer->GetDomain() != nullptr) {
			std::print("Domain: [{}] [{}]   ",
				sorcerer->GetDomain()->GetDomainName(),
				sorcerer->GetDomainStatus());

		}
		if (sorcerer->GetCounterDomain() != nullptr) {
			std::print("Counter: {} [{}]   ",
				sorcerer->GetCounterDomain()->GetDomainName(),
				sorcerer->GetCounterStatus());
		}
		if (sorcerer->GetTechnique() != nullptr) {
			std::print("Technique: [{}] [{}] ",
				sorcerer->GetTechnique()->GetTechniqueName(),
				sorcerer->GetTechnique()->GetStringStatus());
		}
	}
	if (!s->GetCursedTools().empty() || s->GetTool() != nullptr) {
		std::print("Inventory: ");
		if (s->GetCursedTools().empty()) {
			std::println("[{}Empty{}]",Color::DimGray,Color::Clear);
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
			std::println(" [{}None{}]",Color::DimGray,Color::Clear);
		}
	}
	if (s->IsThePlayer() && s->IsCharacterStunned()) {
		std::println("\n\n");
		std::println("You have been Stunned and your turn has been skipped!");
		std::println("\n\n");
	}
	else if (s->IsThePlayer()) {
		std::println("\nChoose action:");

		std::string techniqued = std::format("1 - Technique [{}None{}] ", Color::DimGray, Color::Clear);
		std::string speciald = std::format("3 - Special [{}None{}] ", Color::DimGray, Color::Clear);
		std::string domaind = std::format("4 - Domain [{}Locked{}]", Color::DimGray, Color::Clear);
		std::string settingd = std::format("9 - Technique Settings [{}Locked{}]", Color::DimGray, Color::Clear);
		std::string rctd = std::format("6 - Reverse Cursed Technique [{}Locked{}]", Color::DimGray, Color::Clear);
		std::string amplificationd = std::format("7 - Domain Amplification [{}Locked{}]", Color::DimGray, Color::Clear);
		std::string shikigami = std::format("10 - Shikigami [{}None{}]", Color::DimGray, Color::Clear);

		if (s->IsaCurseUser()) {
			CurseUser* p_cuser = static_cast<CurseUser*>(s);
			Domain* domain = p_cuser->GetDomain();
			Domain* counter = p_cuser->GetCounterDomain();
			Technique* tech = p_cuser->GetTechnique();
			Specials* special = p_cuser->GetSpecial();
			

			techniqued = (tech == nullptr)
				? std::format("1 - Technique [{}None{}] ", Color::DimGray, Color::Clear)
				: std::format("1 - Technique [{}] ", tech->GetTechniqueName());

			speciald = (special == nullptr)
				? std::format("3 - Special [{}None{}] ", Color::DimGray, Color::Clear)
				: std::format("3 - Special [{}] ", special->GetSpecialName());

			domaind = (domain == nullptr && counter == nullptr)
				? std::format("4 - Domain [{}Locked{}]", Color::DimGray, Color::Clear)
				: "4 - Domain Actions";

			settingd = (tech == nullptr)
				? std::format("9 - Technique Settings [{}Locked{}]", Color::DimGray, Color::Clear)
				: "9 - Technique Settings";

			rctd = p_cuser->IsaSorcerer()
				? std::format("6 - Reverse Cursed Technique [{}]", static_cast<Sorcerer*>(p_cuser)->GetRCTstatus())
				: std::format("6 - Reverse Cursed Technique [{}Locked{}]", Color::DimGray, Color::Clear);

			amplificationd = p_cuser->IsPhysicallyGifted()
				? std::format("7 - Domain Amplification [{}Locked{}]", Color::DimGray, Color::Clear)
				: std::format("7 - Domain Amplification [{}]", p_cuser->GetDAstatus());

			shikigami = p_cuser->GetShikigami().empty()
				? std::format("10 - Shikigami [{}None{}]", Color::DimGray, Color::Clear)
				: "10 - Shikigami";
		}

		std::string fightd = "2 - Fight";
		std::string tauntd = "5 - Taunt";
		std::string toold = (s->GetCursedTools().empty() && s->GetTool() == nullptr)
			? std::format("8 - Cursed Tools [{}None{}]", Color::DimGray, Color::Clear)
			: "8 - Cursed Tools";
		std::string reinforcement = s->IsaSorcerer()
			? "11 - Reinforcement Level"
			: std::format("11 - Reinforcement [{}Locked{}]", Color::DimGray, Color::Clear);

		int col_width = 35;
		std::println("  {} | {} | {}",
			ANSIPadding(techniqued, col_width),
			ANSIPadding(fightd, col_width),
			ANSIPadding(speciald, col_width));

		std::println("  {} | {} | {}",
			ANSIPadding(domaind, col_width),
			ANSIPadding(tauntd, col_width),
			ANSIPadding(rctd, col_width));

		std::println("  {} | {} | {}",
			ANSIPadding(amplificationd, col_width),
			ANSIPadding(toold, col_width),
			ANSIPadding(settingd, col_width));

		std::println("  {} | {}",
			ANSIPadding(shikigami, col_width),
			ANSIPadding(reinforcement, col_width));

		std::print("\n=> ");
	}
}
void UserInterface::ContinuePrompt() {
	std::println("Press Enter to continue...");
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cin.get();
}

void UserInterface::ClearScreen() {
	std::print("\033[2J\033[H");
}