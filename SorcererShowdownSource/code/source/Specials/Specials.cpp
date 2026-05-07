#include "code/header/Specials/Specials.h"
#include "code/header/GameManagement/Utils.h"


std::string Specials::GetSpecialName() const {
	return std::format("{}{}{}", special_color, special_name, Color::Clear);
}

bool Specials::IsUnlimitedPurple() const {
	return false;
}
bool Specials::IsWorldCuttingSlash() const {
	return false;
}