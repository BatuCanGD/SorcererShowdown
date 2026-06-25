#include "code/header/GameManagement/PlayerFinder.h"
#include "code/header/Characters/CurseUsers/Sorcerers/Sorcerer.h"
#include "code/header/Characters/CurseUsers/CurseUser.h"

void PlayerType::FindPlayerType(){
	if (!player) return;
	if (player->IsaCurseUser()){
		crs = static_cast<CurseUser*>(player);
		if (crs->IsaSorcerer()){
			src = static_cast<Sorcerer*>(crs);
		}
	}
}