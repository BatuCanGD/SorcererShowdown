#include "code/header/CursedTools/PlayfulCloud.h"
#include "code/header/Characters/CurseUsers/Sorcerers/Sorcerer.h"
#include "code/header/Characters/PhysicallyGifted/PhysicallyGifted.h"
#include "code/header/GameManagement/Colors.h"


PlayfulCloud::PlayfulCloud() {
	name = "Playful Cloud";
	color = "\033[31m";
	base_damage = 35.0;
}

double PlayfulCloud::GetCalculatedStrength(Character* user) const {
	if (user->IsPhysicallyGifted()) {
		return base_damage + (static_cast<PhysicallyGifted*>(user)->GetStrengthDamage() * 2.5);
	}
	else {
		return base_damage; 
	}
}

std::unique_ptr<CursedTool> PlayfulCloud::Clone() const {
	return std::make_unique<PlayfulCloud>(*this);
}