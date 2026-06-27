#include "code/header/Characters/Character.h"
#include "code/header/GameManagement/BattlefieldHeader.h"
#include "code/header/GameManagement/BattleManager.h"
#include "code/header/GameManagement/PlayerManager.h"
#include "code/header/GameManagement/UserInterface.h"

int main() {
	std::ios_base::sync_with_stdio(false);
	bool playing = true;
	while (playing){
		Battlefield bf; BattleCreator bc; 
		BattleManager manager(bf, bc);
		UserInterface interface; PlayerManager player;
		const bool spectator_mode = manager.SetupBattlefield();
		auto [skip_turns, skip_all] = manager.SkipTurnFullyCheck();
		interface.ShowBattleEntry(bf.battlefield);
		if (!spectator_mode) bf.battlefield[0]->SetAsPlayer(true);
		while (true) {
			bool game_over = false;
			for (const auto& s : bf.battlefield) {
				if (s->GetCharacterHealth() <= 0.0) continue;
				interface.DisplaySorcererStatus(s.get());
				std::println("\n");	
				if (s->IsThePlayer()) {
					if (!s->IsCharacterStunned()) player.OnPlayerTurn(s.get(), bf);
				}
				else s->OnCharacterTurn(bf);
				std::println("\n");	
				if (!skip_turns) interface.ContinuePrompt(false);
				game_over = manager.GameEndCheck(spectator_mode);
				if (game_over) break;
			}
			
			bool player_found = manager.PlayerSearch(spectator_mode);
			manager.DomainCheckAndPerform();
			manager.ClearCharacters();
			manager.ManageEndOfTurn();
			if (manager.IsBattleOver(game_over, player_found, spectator_mode)) break;
			manager.SpawnNewFighters();
			if (!skip_all) interface.ContinuePrompt(true);
			interface.ClearScreen();
		}
		if (manager.GameEndChoice()) playing = false;
	}
	return 0;
}