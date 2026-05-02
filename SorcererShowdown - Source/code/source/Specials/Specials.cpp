#include "Specials.h"
#include "Utils.h"


std::string Specials::GetSpecialName() const {
	return std::format("{}{}{}", special_color, special_name, Color::Clear);
}