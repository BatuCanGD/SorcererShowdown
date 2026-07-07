#include "code/header/GameManagement/GameDef.h"
#include "code/header/GameManagement/Utils.h"
#include "code/header/Characters/Character.h"
#include "code/header/GameManagement/BattlefieldHeader.h"
#include "code/header/GameManagement/BattleManager.h"
#include "code/header/GameManagement/PlayerManager.h"
#include "code/header/GameManagement/UserInterface.h"

void Game() {
    Battlefield bf;
    BattleCreator bc;
    BattleManager manager(bf, bc);
    UserInterface interface;
    PlayerManager player;

    const bool spectator_mode = manager.SetupBattlefield();
    auto [skip_turns, skip_all, minput] = manager.SkipCheck();

    interface.ShowBattleEntry(bf.battlefield);
    if (!spectator_mode) {
        bf.battlefield[0]->SetAsPlayer(true);
    }
	
    while (true) {
        for (const auto& s : bf.battlefield) {
            if (s->GetCharacterHealth() <= 0.0) continue;

            if (!minput || s->IsThePlayer()) {
                interface.DisplaySorcererStatus(s.get());
                std::println("\n"); 
            }

            if (s->IsThePlayer()) {
                interface.GetPlayerActions(s.get());
                if (!s->IsCharacterStunned()) {
                    player.OnPlayerTurn(s.get(), bf);
                }
            } else {
                s->OnCharacterTurn(bf);
            }

            if (!minput) {
                std::println("\n");
                if (!skip_turns) interface.ContinuePrompt(false);
            }
        }
        bool game_over = manager.GameEndCheck(spectator_mode);
        bool player_found = manager.PlayerSearch(spectator_mode);

        manager.DomainCheckAndPerform();
        manager.ClearCharacters();
        manager.ManageEndOfTurn(minput);

        if (manager.IsBattleOver(game_over, player_found, spectator_mode)) {
			break;
        }

        manager.SpawnNewFighters();

        if (!skip_all && !minput) interface.ContinuePrompt(true);
        interface.ClearScreen();
    }
}

bool GameEndChoice(){
	std::println("1 - End Game | 2 - Restart");
	return Utilities::GetInput<int>() == 1;
}