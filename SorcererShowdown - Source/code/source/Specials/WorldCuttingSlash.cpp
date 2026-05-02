#include "Mahoraga.h"
#include "Shrine.h"
#include "Sorcerer.h"
#include "WorldCuttingSlash.h"



WorldCuttingSlash::WorldCuttingSlash() {
	special_name = "World Cutting Slash";
	special_color = "\033[31m";
}

void WorldCuttingSlash::PerformSpecial(CurseUser* user) {
	Technique* currentTech = user->GetTechnique();
	Shrine* shrinePtr = dynamic_cast<Shrine*>(currentTech);

	if (shrinePtr == nullptr) return;

	const auto& shikigami_list = user->GetShikigami();
	for (const auto& s : shikigami_list) {
		if (Mahoraga* m = dynamic_cast<Mahoraga*>(s.get())) {
			if (m->FullyAdapted()) {
				shrinePtr->SetWCS(true);
				std::println("The blueprint is complete. World Cutting Slash enabled!");
				return;
			}
			else {
				std::println("The blueprint is incomplete. World Cutting Slash cannot be used yet.");
			}
		}
	}
}

std::unique_ptr<Specials> WorldCuttingSlash::Clone() const {
	return std::make_unique<WorldCuttingSlash>(*this);
}