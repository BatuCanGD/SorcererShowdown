#include "code/header/CursedTools/PlayfulCloud.h"
#include "code/header/Characters/CurseUsers/Sorcerers/Sorcerer.h"
#include "code/header/Characters/PhysicallyGifted/PhysicallyGifted.h"
#include "code/header/GameManagement/Utils.h"


PlayfulCloud::PlayfulCloud() {
	name = "Playful Cloud";
	color = "\033[31m";
	base_damage = 65.0;
}
void PlayfulCloud::UseTool(Character* user, Character* target) {
	target->Damage(GetCalculatedStrength(user));
	std::println("{} {}attacks{} {} with {}", user->GetNameWithID(), Utilities::Color::Red, Utilities::Color::Clear, target->GetNameWithID(), this->GetName());
}

std::unique_ptr<CursedTool> PlayfulCloud::Clone() const {
	return std::make_unique<PlayfulCloud>(*this);
}