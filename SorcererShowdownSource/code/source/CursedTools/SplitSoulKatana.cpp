#include "code/header/CursedTools/SplitSoulKatana.h"
#include "code/header/Characters/CurseUsers/Sorcerers/Sorcerer.h"
#include "code/header/Characters/PhysicallyGifted/PhysicallyGifted.h"
#include "code/header/GameManagement/Colors.h"

SplitSoulKatana::SplitSoulKatana() {
	name = "Split Soul Katana";
	color = "\033[35m";
	type = WeaponType::ReinforcementBypass;
	base_damage = 60.0;
}

std::unique_ptr<CursedTool> SplitSoulKatana::Clone() const {
	return std::make_unique<SplitSoulKatana>(*this);
}