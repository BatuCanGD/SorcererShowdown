#include "PlayfulCloud.h"
#include "Sorcerer.h"
#include "PhysicallyGifted.h"
#include "Utils.h"


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