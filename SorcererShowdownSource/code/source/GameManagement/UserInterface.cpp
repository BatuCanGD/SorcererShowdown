#include "code/header/GameManagement/UserInterface.h"
#include "code/header/Characters/Shikigami/Shikigami.h"
#include "code/header/Techniques/Techniques.h"
#include "code/header/CursedTools/CursedTool.h"
#include "code/header/Specials/Specials.h"
#include "code/header/Domains/Domain.h"
#include "code/header/Characters/CurseUsers/Sorcerers/Sorcerer.h"
#include "code/header/GameManagement/Colors.h"
#include "code/header/GameManagement/Utils.h"

namespace UserInterface{

	std::string StringPad(std::string_view string, size_t length) {
		size_t len = 0; 
		bool ansi = false;
		for (char c : string) {
			if (c == '\033') { ansi = true; }
			else if (ansi) { if (c == 'm') { ansi = false; } }
			else { len++; }
		}
		std::string result(string);
		if (len < length) { 
			result.append(length - len, ' ');
		}
		return result;
	}

	void ShowBattleEntry(const std::vector<std::unique_ptr<Character>>& bf){
		const size_t size = bf.size();
		if (size == 2) {
			std::println("Its just you and {}. Defeat them and win", bf[1]->GetName());
		}
		else if (size > 9) {
			std::println("You, {}, and {} more sorcerers are in the area. Defeat all of them to win", bf[1]->GetName(), bf.size() - 2);
		}
		else {
			std::println("You and {} more sorcerers are near you. Defeat them all to win", bf.size() - 1);
		}

		std::println("-------Let the battle between {} sorcerers begin!-------", size);
		std::println("-------------------------------------------------------");
	}

	void DisplaySorcererStatus(Character* s){
		std::println("-------------{}'s Turn-------------- {}", 
					s->IsThePlayer() ? "Player's (" + s->GetNameWithID() + ")" : s->GetNameWithID(), 
					s->IsCharacterStunned() ? "(Stunned)" : "");
		double hp = s->GetCharacterHealth();
		double max_hp = s->GetCharacterMaxHealth();
		std::string_view hp_color = Color::Green;

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
			std::string_view ce_color = crs->CEMoreThanMax(0.10) ? Color::Cyan : Color::DimGray;

			if (crs->GetTechnique() != nullptr) {
				std::print("Cursed Energy [{}{:.1f}/{:.1f}{}] | Technique [{}]:[{}]-[{}]\n",
				ce_color, ce, max_ce, Color::Clear, 
				crs->GetTechnique()->GetTechniqueName(),
				crs->GetTechnique()->GetStringStatus(),
				crs->GetTechnique()->GetStringChantLevel());
			}else{
				std::println("Cursed Energy [{}{:.1f}/{:.1f}{}]", ce_color, ce, max_ce, Color::Clear);
			}

			if (src) {
				std::print("Domain Amp [{}] | Reverse Cursed Technique [{}] | Reinforcement [{}]",
					src->GetDAstatus(), src->GetRCTstatus(), src->GetReinforcementStatus());
			}
			else { std::print("Domain Amp [{}] | Reinforcement [{}]", 
				crs->GetDAstatus(), crs->GetReinforcementStatus());
			}
			std::println();

			if (Domain* domain = crs->GetDomain()) {
				std::print("Domain [{}]:[{}] | ",
					domain->GetDomainName(),
					domain->GetDomainStatus());
			}
			if (Domain* counter = crs->GetCounter()) {
				std::print("Counter [{}]:[{}] | ",
					counter->GetDomainName(),
					counter->GetDomainStatus());
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
				std::println();
			}
			std::print("Current Tool ");

			if (s->GetTool() != nullptr) {
				std::println("[{}]", s->GetTool()->GetName());
			}
			else {
				std::println("[{}None{}]",Color::DimGray,Color::Clear);
			}
		}
	}

	void GetPlayerActions(Character* s){
		if (s->IsCharacterStunned()){
			std::println("\n\nYou have been Stunned and your turn has been skipped!\n\n");
			return;
		}
		auto* crs = s->IsaCurseUser() ? static_cast<CurseUser*>(s) : nullptr;
		auto* src = (crs && crs->IsaSorcerer()) ? static_cast<Sorcerer*>(crs) : nullptr;
		
		std::println("\nChoose action:");
		
		std::string techniqued = (crs && crs->GetTechnique())
		? std::format("1 - Technique [{}] ", crs->GetTechnique()->GetTechniqueName())
		: std::format("1 - Technique [{}None{}] ", Color::DimGray, Color::Clear); 

		std::string fightd = "2 - Fight"; 

		std::string speciald = (crs && crs->GetSpecial())
			? std::format("3 - Special [{}] ", crs->GetSpecial()->GetSpecialName())
			: std::format("3 - Special [{}None{}] ", Color::DimGray, Color::Clear); 

		std::string domaind = (crs && (crs->GetDomain() || crs->GetCounter()))
			? "4 - Domain Actions"
			: std::format("4 - Domain [{}Locked{}]", Color::DimGray, Color::Clear); 

		std::string tauntd = "5 - Taunt"; 

		std::string rctd = src
			? std::format("6 - Reverse Cursed Technique [{}]", src->GetRCTstatus())
			: std::format("6 - Reverse Cursed Technique [{}Locked{}]", Color::DimGray, Color::Clear); 

		std::string amplificationd = crs
			? std::format("7 - Domain Amplification [{}]", crs->GetDAstatus())
			: std::format("7 - Domain Amplification [{}Locked{}]", Color::DimGray, Color::Clear); 

		std::string toold = (s->GetCursedTools().empty() && s->GetTool() == nullptr)
			? std::format("8 - Cursed Tools [{}None{}]", Color::DimGray, Color::Clear)
			: "8 - Cursed Tools"; 

		std::string settingd = (crs && crs->GetTechnique())
			? "9 - Technique Settings"
			: std::format("9 - Technique Settings [{}Locked{}]", Color::DimGray, Color::Clear); 

		std::string shikigami = (crs && !crs->GetShikigami().empty())
			? "10 - Shikigami"
			: std::format("10 - Shikigami [{}None{}]", Color::DimGray, Color::Clear); 

		std::string reinforcement = crs
			? "11 - Reinforcement Level"
			: std::format("11 - Reinforcement [{}Locked{}]", Color::DimGray, Color::Clear); 

		std::string bindingvows = crs ? "12 - Binding Vows" : std::format("12 - Binding Vows [{}Locked{}]", Color::DimGray, Color::Clear); 

		constexpr size_t col_width = 35;
		std::println("  {} | {} | {}", StringPad(techniqued, col_width), StringPad(fightd, col_width), StringPad(speciald, col_width));
		std::println("  {} | {} | {}", StringPad(domaind, col_width), StringPad(tauntd, col_width), StringPad(rctd, col_width));
		std::println("  {} | {} | {}", StringPad(amplificationd, col_width), StringPad(toold, col_width), StringPad(settingd, col_width));
		std::println("  {} | {} | {}", StringPad(shikigami, col_width), StringPad(reinforcement, col_width), StringPad(bindingvows, col_width));
		std::print("\n=> ");
	}


	void ContinuePrompt(bool end_of_round){
		std::println("Press Enter to {}...", end_of_round ? "begin the next round" : "continue");
		Utilities::WaitForInput();
	}

	void ClearScreen(){
		std::print("\033[2J\033[H");
	}
};