#include "code/header/CursedTools/Katana.h"
#include "code/header/Characters/PhysicallyGifted/PhysicallyGifted.h"
#include "code/header/Characters/CurseUsers/Sorcerers/Sorcerer.h"
#include "code/header/GameManagement/Utils.h"



Katana::Katana() {
	name = "a Katana";
	color = "\033[36m";
	base_damage = 50.0;
}
void Katana::UseTool(Character* user, Character* target) {
	target->Damage(GetCalculatedStrength(user));
	std::println("{} attacks {} using {}", user->GetNameWithID(), target->GetNameWithID(), this->GetName());
}

std::unique_ptr<CursedTool> Katana::Clone() const {
	return std::make_unique<Katana>(*this);
}