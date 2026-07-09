#include "code/header/CursedTools/Katana.h"

Katana::Katana() {
	name = "CE Infused Katana";
	color = "\033[36m";
	base_damage = 50.0;
}

std::unique_ptr<CursedTool> Katana::Clone() const {
	return std::make_unique<Katana>(*this);
}