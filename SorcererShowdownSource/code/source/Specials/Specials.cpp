#include "code/header/Specials/Specials.h"
#include "code/header/GameManagement/Utils.h"

Specials::~Specials() = default;

std::string Specials::GetSpecialName() const {
	return std::format("{}{}{}", color, name, Utilities::Color::Clear);
}
bool Specials::IsUnlimitedPurple() const {
	return false;
}
bool Specials::IsWorldCuttingSlash() const {
	return false;
}