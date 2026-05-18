#include "code/header/CursedTools/SplitSoulKatana.h"
#include "code/header/Characters/CurseUsers/Sorcerers/Sorcerer.h"
#include "code/header/Characters/PhysicallyGifted/PhysicallyGifted.h"
#include "code/header/GameManagement/Utils.h"

SplitSoulKatana::SplitSoulKatana() {
	name = "Split Soul Katana";
	color = "\033[35m";
	base_damage = 60.0;
}

void SplitSoulKatana::UseTool(Character* user, Character* target) {
	target->DamageBypassReinforcement(GetCalculatedStrength(user));
	std::println("{} {}attacks{} {}'s soul with {}", user->GetNameWithID(), Utilities::Color::Red, Utilities::Color::Clear, target->GetNameWithID(), this->GetName());
}

std::unique_ptr<CursedTool> SplitSoulKatana::Clone() const {
	return std::make_unique<SplitSoulKatana>(*this);
}