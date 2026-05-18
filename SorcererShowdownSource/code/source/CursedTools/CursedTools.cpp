#include "code/header/CursedTools/CursedTool.h"
#include "code/header/Characters/PhysicallyGifted/PhysicallyGifted.h"
#include "code/header/Characters/CurseUsers/CurseUser.h"
#include "code/header/GameManagement/Utils.h"


double CursedTool::GetCalculatedStrength(Character* user)  const {
	if (user->IsPhysicallyGifted()) { 
		return base_damage + (static_cast<PhysicallyGifted*>(user)->GetStrengthDamage() * 2.5);
	}
	else {
		return base_damage + ((std::sqrt(static_cast<CurseUser*>(user)->GetCharacterCE()) * 1.5) * 0.67);
	}
}

std::string CursedTool::GetName() const {
	return std::format("{}{}{}", color, name, Utilities::Color::Clear);
}
std::string CursedTool::GetSimpleName()const {
	return name;
}

bool CursedTool::IsAntiTechniqueWeapon() const {
	return false;
}