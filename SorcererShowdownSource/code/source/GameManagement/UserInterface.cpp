#include "code/header/GameManagement/UserInterface.h"
#include "code/header/Characters/Shikigami/Shikigami.h"
#include "code/header/Techniques/Techniques.h"
#include "code/header/CursedTools/CursedTool.h"
#include "code/header/Specials/Specials.h"
#include "code/header/Domains/Domain.h"
#include "code/header/Characters/CurseUsers/Sorcerers/Sorcerer.h"
#include "code/header/GameManagement/Colors.h"
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

std::string StringPad(const std::string& string, size_t length) {
	size_t len = 0; bool ansi = false;
	for (char c : string) {
		if (c == '\033') { ansi = true; }
		else if (ansi) { if (c == 'm') { ansi = false; } }
		else { len++; }
	}
	if (len < length) { return string + std::string(length - len, ' '); }
	return string;
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

	if (!s->HPMoreThanMax(0.10)) hp_color = Color::Red;
	else if (!s->HPMoreThanMax(0.33)) hp_color = Color::BrightRed;
	else if (!s->HPMoreThanMax(0.66)) hp_color = Color::Yellow;
	else hp_color = Color::Green;
	
	std::print("Health [{}{:.1f}/{:.1f}{}] | ",hp_color, hp, max_hp, Color::Clear);

	CurseUser* crs = s->IsaCurseUser() ? static_cast<CurseUser*>(s) : nullptr;
    Sorcerer* src = (crs && s->IsaSorcerer()) ? static_cast<Sorcerer*>(crs) : nullptr;
	
	if (s->IsaCurseUser()) {
		double ce = crs->GetCharacterCE();
		double max_ce = crs->GetCharacterMaxCE();
		std::string ce_color = Color::Cyan;

		if (!crs->CEMoreThanMax(0.10)) {
			ce_color = Color::DimGray;
		}

		if (crs->GetTechnique() != nullptr) {
			std::print("Cursed Energy [{}{:.1f}/{:.1f}{}] | Technique [{}]:[{}]-[{}]\n",
			ce_color, ce, max_ce, Color::Clear, 
			crs->GetTechnique()->GetTechniqueName(),
			crs->GetTechnique()->GetStringStatus(),
			crs->GetTechnique()->GetStringChantLevel());
		}else{
			std::println("Cursed Energy [{}{:.1f}/{:.1f}{}]", ce_color, ce, max_ce, Color::Clear);
		}
		if (crs->IsaSorcerer()) {
			std::print("Domain Amp [{}] | Reverse Cursed Technique [{}] | Reinforcement [{}]",
				src->GetDAstatus(), src->GetRCTstatus(), src->GetReinforcementStatus());
		}
		else { std::print("Domain Amp [{}] | Reinforcement [{}]", 
			crs->GetDAstatus(), crs->GetReinforcementStatus());
		}
		std::println("");
		if (crs->GetDomain()) {
			std::print("Domain [{}]:[{}] | ",
				crs->GetDomain()->GetDomainName(),
				crs->GetDomainStatus());
		}
		if (crs->GetCounterDomain()) {
			std::print("Counter [{}]:[{}] | ",
				crs->GetCounterDomain()->GetDomainName(),
				crs->GetCounterStatus());
		}
	}
	if (!s->GetCursedTools().empty() || s->GetTool() != nullptr) {
		std::print("Inventory ");
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
	if (s->IsThePlayer()) {
		if (s->IsCharacterStunned()){
			std::println("\n\n");
			std::println("You have been Stunned and your turn has been skipped!");
			std::println("\n\n");
			return;
		}
		std::println("\nChoose action:");
		std::string techniqued = std::format("1 - Technique [{}None{}] ", Color::DimGray, Color::Clear);
		std::string speciald = std::format("3 - Special [{}None{}] ", Color::DimGray, Color::Clear);
		std::string domaind = std::format("4 - Domain [{}Locked{}]", Color::DimGray, Color::Clear);
		std::string settingd = std::format("9 - Technique Settings [{}Locked{}]", Color::DimGray, Color::Clear);
		std::string rctd = std::format("6 - Reverse Cursed Technique [{}Locked{}]", Color::DimGray, Color::Clear);
		std::string amplificationd = std::format("7 - Domain Amplification [{}Locked{}]", Color::DimGray, Color::Clear);
		std::string shikigami = std::format("10 - Shikigami [{}None{}]", Color::DimGray, Color::Clear);
		std::string bindingvows = std::format("12 - Binding Vows [{}Locked{}]", Color::DimGray, Color::Clear);
		if (crs) {
			Domain* domain = crs->GetDomain();
			Domain* counter = crs->GetCounterDomain();
			Technique* tech = crs->GetTechnique();
			Specials* special = crs->GetSpecial();
			

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

			rctd = src
				? std::format("6 - Reverse Cursed Technique [{}]", src->GetRCTstatus())
				: std::format("6 - Reverse Cursed Technique [{}Locked{}]", Color::DimGray, Color::Clear);

			amplificationd = std::format("7 - Domain Amplification [{}]", crs->GetDAstatus());

			shikigami = crs->GetShikigami().empty()
				? std::format("10 - Shikigami [{}None{}]", Color::DimGray, Color::Clear)
				: "10 - Shikigami";
			bindingvows = "12 - Binding Vows";
		}

		std::string fightd = "2 - Fight";
		std::string tauntd = "5 - Taunt";
		std::string toold = (s->GetCursedTools().empty() && s->GetTool() == nullptr)
			? std::format("8 - Cursed Tools [{}None{}]", Color::DimGray, Color::Clear)
			: "8 - Cursed Tools";
		std::string reinforcement = crs
			? "11 - Reinforcement Level"
			: std::format("11 - Reinforcement [{}Locked{}]", Color::DimGray, Color::Clear);

		size_t col_width = 35;
		std::println("  {} | {} | {}",
			StringPad(techniqued, col_width),
			StringPad(fightd, col_width),
			StringPad(speciald, col_width));

		std::println("  {} | {} | {}",
			StringPad(domaind, col_width),
			StringPad(tauntd, col_width),
			StringPad(rctd, col_width));

		std::println("  {} | {} | {}",
			StringPad(amplificationd, col_width),
			StringPad(toold, col_width),
			StringPad(settingd, col_width));

		std::println("  {} | {} | {}",
			StringPad(shikigami, col_width),
			StringPad(reinforcement, col_width),
			StringPad(bindingvows, col_width));

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