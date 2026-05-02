#include "TransfiguredHuman.h"
#include "BattlefieldHeader.h"
#include "CursedSpirit.h"
#include "Utils.h"



TransfiguredHuman::TransfiguredHuman() : CursedSpirit(50.0, 10.0, 0.0) {
	passive_health_regen = 0.0;
	current_ce_reinforcement = 0.0;
	max_ce_reinforcement = 0.0;
	black_flash_chance = 0;

	char_name = "Transfigured Human";
};

void TransfiguredHuman::OnCharacterTurn(Character*, Battlefield& bf) {
	if (this->IsCharacterStunned()) {
		std::println("{} is stunned and their turn will be skipped", this->GetNameWithID());
		return;
	}
	Character* target = nullptr;

	for (const auto& tar : bf.battlefield) {

		if (tar.get() != this)
		{
			if (!target) {
				target = tar.get();
				continue;
			} 
			if (tar->IsaCursedSpirit()) {
				continue;
			}
			if (GetRandomNumber(0, 1) == 1) {
				target = tar.get();
			}
		}
	}
	if (!target) {
		std::println("The transfigured human just stands menacingly");
		return;
	}
	this->Attack(target);
}

bool TransfiguredHuman::IsTransfigured() const {
	return true;
}