#include "code/header/CursedTools/Katana.h"
#include "code/header/Characters/PhysicallyGifted/PhysicallyGifted.h"
#include "code/header/Characters/CurseUsers/Sorcerers/Sorcerer.h"
#include "code/header/GameManagement/Utils.h"



Katana::Katana() {
	tool_name = "a Katana";
	tool_color = "\033[36m";
}
void Katana::UseTool(Character* user, Character* target) {
	double total_damage = 20.0;
	if (user->IsPhysicallyGifted()) {
		auto pg = static_cast<PhysicallyGifted*>(user);
		total_damage = (base_tool_damage * special_tool_mult) * pg->GetStrengthDamage();
	}
	else if (user->IsaCurseUser()) {
		auto sr = static_cast<CurseUser*>(user);
		total_damage = (base_tool_damage * special_tool_mult) * (1 + (sr->GetCharacterCE() / 5000));
	}

	target->Damage(total_damage);
	std::println("{} attacks {} using {}", user->GetNameWithID(), target->GetNameWithID(), this->GetName());
}

std::unique_ptr<CursedTool> Katana::Clone() const {
	return std::make_unique<Katana>(*this);
}