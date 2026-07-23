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

			if (src && src->HasRCT()) {
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
	
	void ContinuePrompt(bool end_of_round){
		std::println("Press Enter to {}...", end_of_round ? "begin the next round" : "continue");
		Utilities::WaitForInput();
	}

	void ClearScreen(){
		std::print("\033[2J\033[H");
	}
};