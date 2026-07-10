#include "code/header/Characters/CurseUsers/CursedSpirits/TransfiguredHuman.h"
#include "code/header/GameManagement/BattlefieldHeader.h"
#include "code/header/GameManagement/Utils.h"

TransfiguredHuman::TransfiguredHuman() : CursedSpirit(Utilities::GetRandom<double>(1.0, 60.0), 10.0, 0.0) {
	passive_health_regen = 0.0;
	current_ce_reinforcement = 0.0;
	max_reinforcement = 0.0;
	attack_damage = Utilities::GetRandom<double>(5.0, 60.0);
	black_flash_chance = 0;
	name = "Transfigured Human";
}

void TransfiguredHuman::OnCharacterTurn(Battlefield& bf) {
	if (IsCharacterStunned()) {
		std::println("{} is stunned and their turn will be skipped", GetNameWithID());
		return;
	}
	Character* target = nullptr;

	for (const auto& tar : bf.battlefield) {
		if (tar.get() == this || tar->IsaCursedSpirit()) continue;
		if (Utilities::GetRandom<int>(1, 100) >= 65) {
			target = tar.get();
			continue;
		} 
	}
	if (!target) {
		std::println("The transfigured human stands, confused");
		return;
	}
	Attack(target);
}

bool TransfiguredHuman::IsTransfigured() const {
	return true;
}