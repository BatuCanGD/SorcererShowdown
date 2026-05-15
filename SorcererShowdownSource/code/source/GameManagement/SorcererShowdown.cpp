#include "code/header/Characters/Character.h"
#include "code/header/GameManagement/BattlefieldHeader.h"
#include "code/header/GameManagement/BattleManager.h"
#include "code/header/GameManagement/PlayerManager.h"
#include "code/header/GameManagement/UserInterface.h"

int main() {
	Battlefield bf; BattleCreator bc; BattleManager manager;
	PlayerManager player; UserInterface interface;

	bool spectator_mode = manager.SetupBattlefield(bf, bc);
	auto [skip_turns, skip_all] = manager.SkipTurnFullyCheck();
	interface.ShowBattleEntry(bf.battlefield);
	
	if (!spectator_mode) {
		bf.battlefield[0]->SetAsPlayer(true);
	}
	while (true) {
		bool game_over = false;
		for (const auto& s : bf.battlefield) {
			if (s->GetCharacterHealth() <= 0.0) continue;
			if (s->IsThePlayer()) {
				interface.DisplaySorcererStatus(s.get());
				if (s->IsCharacterStunned()) continue;
				std::println("\n");
				player.OnPlayerTurn(*s, bf);
				std::println("\n");
			}
			else {
				std::println("\n");
				interface.DisplaySorcererStatus(s.get());
				std::println("\n");
				s->OnCharacterTurn(s.get(), bf);
				std::println("\n");
			}

			if (manager.GameEndCheck(bf, spectator_mode)) {
				game_over = true;
				break;
			}
			if (!skip_turns) {
				interface.ContinuePrompt(false);
			}
		}
		manager.DomainCheckAndPerform(bf);
		bool player_found = manager.ManageEndOfTurn(bf, spectator_mode);
		manager.SpawnNewFighters(bf);
		
		if (manager.IsBattleOver(game_over, player_found, spectator_mode, bf)) break;
		if (!skip_all) {
			interface.ContinuePrompt(true);
		}
		interface.ClearScreen();
	}
	return manager.EndGame();
}