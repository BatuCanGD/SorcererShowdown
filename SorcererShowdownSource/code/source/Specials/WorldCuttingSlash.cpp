#include "code/header/Characters/Shikigami/Mahoraga.h"
#include "code/header/Techniques/Shrine.h"
#include "code/header/Characters/CurseUsers/Sorcerers/Sorcerer.h"
#include "code/header/Specials/WorldCuttingSlash.h"



WorldCuttingSlash::WorldCuttingSlash() {
	special_name = "World Cutting Slash";
	special_color = "\033[31m";
}

void WorldCuttingSlash::PerformSpecial(CurseUser* user) {
	if (!user->GetTechnique() || !user->GetTechnique()->IsShrine()) return;

	auto shrine = static_cast<Shrine*>(user->GetTechnique());
	if (shrine->WorldCuttingSlashUnlocked()) return;

	const auto& shikigami_list = user->GetShikigami();
	for (const auto& s : shikigami_list) {
		if (s->IsMahoraga()) {
			auto m = static_cast<Mahoraga*>(s.get());
			if (m->FullyAdapted()) {
				shrine->SetWCS(true);
				std::println("The blueprint is complete. The World Cutting Slash can be used!");
				return;
			}
			else {
				std::println("The blueprint is incomplete. The World Cutting Slash cannot be used yet.");
				return;
			}
		}
	}
	std::println("The World Cutting Slash cannot be unlocked, a core piece is missing!");
}

std::unique_ptr<Specials> WorldCuttingSlash::Clone() const {
	return std::make_unique<WorldCuttingSlash>(*this);
}

bool WorldCuttingSlash::IsWorldCuttingSlash() const {
	return true;
}