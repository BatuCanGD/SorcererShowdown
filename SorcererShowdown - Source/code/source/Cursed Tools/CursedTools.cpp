#include "CursedTool.h"
#include "PhysicallyGifted.h"
#include "Utils.h"


double CursedTool::GetCalculatedStrength(Character* user)  const {
	if (user->IsPhysicallyGifted()) {
		auto pg = static_cast<PhysicallyGifted*>(user);
		return base_tool_damage + (pg->GetStrengthDamage() * 1.25);
	}
	else {
		return base_tool_damage + (user->GetCharacterMaxHealth() / 10.0);
	}
}

std::string CursedTool::GetName() const {
	return std::format("{}{}{}", tool_color, tool_name, Color::Clear);
}
std::string CursedTool::GetSimpleName()const {
	return tool_name;
}

bool CursedTool::IsAntiTechniqueWeapon() const {
	return false;
}