#include "code/header/CursedTools/SplitSoulKatana.h"
#include "code/header/Characters/CurseUsers/Sorcerers/Sorcerer.h"
#include "code/header/Characters/PhysicallyGifted/PhysicallyGifted.h"
#include "code/header/GameManagement/Utils.h"

SplitSoulKatana::SplitSoulKatana() {
	tool_name = "Split Soul Katana";
	tool_color = "\033[35m";
}

void SplitSoulKatana::UseTool(Character* user, Character* target) {
	double dmg = GetCalculatedStrength(user);
	target->DamageBypassReinforcement(dmg);
	std::println("{} {}attacks{} {}'s soul with {}", user->GetNameWithID(), Utilities::Color::Red, Utilities::Color::Clear, target->GetNameWithID(), this->GetName());
}

std::unique_ptr<CursedTool> SplitSoulKatana::Clone() const {
	return std::make_unique<SplitSoulKatana>(*this);
}