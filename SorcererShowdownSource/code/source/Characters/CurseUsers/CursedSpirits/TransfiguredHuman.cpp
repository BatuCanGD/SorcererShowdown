#include "Characters/CurseUsers/CursedSpirits/TransfiguredHuman.h"
#include "GameManagement/BattlefieldHeader.h"
#include "GameManagement/Utils.h"

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
	Character* target = bf.battlefield[Utilities::GetRandom<size_t>(0, bf.battlefield.size() - 1)].get();
    if (target == this) {
        std::println("The transfigured human stands in confusion");
		return;
    }
	Attack(target);
}

bool TransfiguredHuman::IsTransfigured() const {
	return true;
}