#include "code/header/Specials/Specials.h"
#include "code/header/GameManagement/Utils.h"

Specials::~Specials() = default;

std::string Specials::GetSpecialName() const {
	return std::format("{}{}{}", special_color, special_name, Utilities::Color::Clear);
}
bool Specials::IsUnlimitedPurple() const {
	return false;
}
bool Specials::IsWorldCuttingSlash() const {
	return false;
}