#include "GameManagement/GameDef.h"
#include "GameManagement/Utils.h"
#include "GameManagement/BattlefieldHeader.h"
#include "GameManagement/BattleManager.h"
#include "GameManagement/PlayerManager.h"
#include "GameManagement/UserInterface.h"
#include "Characters/Character.h"

struct GameChoices{
    bool spectator_mode{};
    bool skip_turns{};
    bool skip_all{};
    bool minimum_output{};
};

bool RunTurn(Battlefield& bf, BattleManager& mg, PlayerManager& pm, const GameChoices& gc){
    for (const auto& s : bf.battlefield) {
        if (s->GetCharacterHealth() <= 0.0) continue;

        if (!gc.minimum_output || s->IsThePlayer()) {
            UserInterface::DisplaySorcererStatus(s.get());
            std::println("\n"); 
        }

        if (s->IsThePlayer()) {
            pm.OnPlayerTurn(s.get(), bf);
        } else {
            s->OnCharacterTurn(bf);
        }

        if (!gc.minimum_output) {
            std::println("\n");
            if (!gc.skip_turns) UserInterface::ContinuePrompt(false);
        }
    }
    const bool game_over = mg.GameEndCheck(gc.spectator_mode);
    const bool player_found = mg.PlayerSearch(gc.spectator_mode);

    mg.DomainCheckAndPerform();
    mg.ClearCharacters();
    mg.ManageEndOfTurn(gc.minimum_output);

    if (mg.IsBattleOver(game_over, player_found, gc.spectator_mode)) {
        return false;
    }

    mg.SpawnNewFighters();

    if (!gc.skip_all && !gc.minimum_output) UserInterface::ContinuePrompt(true);
    UserInterface::ClearScreen();
    return true;
}

void RunGame() {
    Battlefield bf;BattleCreator bc;
    BattleManager manager(bf, bc);
    PlayerManager player;

    const auto [chosen, spectator_mode] = manager.SetupBattlefield();
    const auto [skip_turns, skip_all, minput] = manager.SkipCheck();

    const GameChoices choices{
        .spectator_mode = spectator_mode, 
        .skip_turns = skip_turns, 
        .skip_all = skip_all, 
        .minimum_output = minput 
    };

    UserInterface::ShowBattleEntry(bf.battlefield);
    if (!spectator_mode && chosen) chosen->SetAsPlayer(true);

    while(RunTurn(bf, manager, player, choices));
}

bool GameEndChoice(){
	std::println("1 - End Game | 2 - Restart");
	return Utilities::GetInput<int>() == 1;
}