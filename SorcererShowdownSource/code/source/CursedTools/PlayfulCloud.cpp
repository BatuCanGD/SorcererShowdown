#include "code/header/CursedTools/PlayfulCloud.h"
#include "code/header/Characters/CurseUsers/Sorcerers/Sorcerer.h"
#include "code/header/Characters/PhysicallyGifted/PhysicallyGifted.h"
#include "code/header/GameManagement/Utils.h"


PlayfulCloud::PlayfulCloud() {
	tool_name = "Playful Cloud";
	tool_color = "\033[31m";
}
void PlayfulCloud::UseTool(Character* user, Character* target) {
	double dmg = GetCalculatedStrength(user);
	target->Damage(dmg);
	std::println("{} {}attacks{} {} with {}", user->GetNameWithID(), Color::Red, Color::Clear, target->GetNameWithID(), this->GetName());
}

std::unique_ptr<CursedTool> PlayfulCloud::Clone() const {
	return std::make_unique<PlayfulCloud>(*this);
}