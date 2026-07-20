#include "Specials/Specials.h"
#include "GameManagement/Colors.h"

Specials::~Specials() = default;

std::string Specials::GetSpecialName() const {
	return std::format("{}{}{}", color, name, Color::Clear);
}
bool Specials::IsUnlimitedPurple() const {
	return false;
}
bool Specials::IsWorldCuttingSlash() const {
	return false;
}