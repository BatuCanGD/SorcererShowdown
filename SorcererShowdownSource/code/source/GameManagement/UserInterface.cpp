#include "GameManagement/UserInterface.h"
#include "GameManagement/Colors.h"
#include "GameManagement/Utils.h"

#include "Characters/Shikigami/Shikigami.h"
#include "Characters/CurseUsers/Sorcerers/Sorcerer.h"

#include "Techniques/Techniques.h"
#include "CursedTools/CursedTool.h"
#include "Specials/Specials.h"
#include "Domains/Domain.h"


namespace UserInterface{
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
			s->IsCharacterStunned() ? "(Stunned)" : ""
		);

		std::string hp_color = Color::Green;

		if (!s->HPMoreThanMax(0.10)) {
			hp_color = Color::Red;
	 	} else if (!s->HPMoreThanMax(0.33)) {
			hp_color = Color::BrightRed;
		} else if (!s->HPMoreThanMax(0.66)) { 
			hp_color = Color::Yellow;
		}
		std::print("Health [{}{:.1f}/{:.1f}{}] ",hp_color, s->GetCharacterHealth(), s->GetCharacterMaxHealth(), Color::Clear);

		if (s->IsaCurseUser()) {
			CurseUser* crs = static_cast<CurseUser*>(s);
			const std::string ce_color = crs->CEMoreThanMax(0.025) ? Color::Cyan : Color::DimGray;

			std::print("| Cursed Energy [{}{:.1f}/{:.1f}{}] ", ce_color, crs->GetCharacterCE(), crs->GetCharacterMaxCE(), Color::Clear);

			if (const Technique* tech = crs->GetTechnique()) {
				std::print("| Technique [{}]:[{}]-[{}]", tech->GetTechniqueName(), tech->GetStringStatus(), tech->GetStringChantLevel());
			}

			std::print("\nDomain Amp [{}] | Reinforcement [{}]", crs->GetDAstatus(), crs->GetReinforcementStatus());

			if (crs->IsaSorcerer()) {
				Sorcerer* src = static_cast<Sorcerer*>(crs);
				if (src->HasRCT()){
					std::print(" | RCT [{}]", src->GetRCTstatus());
				}
			}
			
			std::println();

			if (const Domain* domain = crs->GetDomain()) {
				std::print("Domain [{}]:[{}] | ", domain->GetDomainName(), domain->GetDomainStatus());
			}
			if (const Domain* counter = crs->GetCounter()) {
				std::print("Counter [{}]:[{}] | ", counter->GetDomainName(), counter->GetDomainStatus());
			}
		}
		if (!s->GetCursedTools().empty() || s->GetTool()) {
			std::print("\nInventory [ ");
			if (s->GetCursedTools().empty()) {
				std::print("[{}Empty{}] ",Color::DimGray,Color::Clear);
			}
			else {
				for (const auto& t : s->GetCursedTools()) {
					std::print("[{}] ", t->GetName());
				}
			}
			std::print("]\nCurrent Tool ");

			if (s->GetTool()) {
				std::println("[{}]", s->GetTool()->GetName());
			}
			else {
				std::println("[{}None{}]",Color::DimGray,Color::Clear);
			}
		}
	}
	
	void ContinuePrompt(bool end_of_round){
		std::println("Press Enter to {}...", end_of_round ? "begin the next round" : "continue");
		Utilities::WaitForInput();
	}

	void ClearScreen(){
		std::print("\033[2J\033[H");
	}
};