#include "code/header/Characters/Shikigami/Mahoraga.h"
#include "code/header/Techniques/Shrine/Shrine.h"
#include "code/header/Techniques/Shrine/Dismantle.h"
#include "code/header/GameManagement/VList.h"
#include "code/header/Characters/CurseUsers/Sorcerers/Sorcerer.h"
#include "code/header/Specials/WorldCuttingSlash.h"
#include "code/header/GameManagement/Colors.h"



WorldCuttingSlash::WorldCuttingSlash() {
	name = "World Cutting Slash";
	color = "\033[31m";
}

bool WorldCuttingSlash::CheckSpecial(CurseUser* user) {
	bool has_mahoraga = false, mahoraga_is_fully_adapted = false;
	bool has_shrine = user->GetTechnique() && user->GetTechnique()->IsShrine();
	bool player = user->IsThePlayer(); auto* tech = user->GetTechnique();
	
	for (const auto& s : user->GetShikigami()) {
		if (s->IsMahoraga()) {
			has_mahoraga = true;
			if (static_cast<Mahoraga*>(s.get())->FullyAdapted()) {
				mahoraga_is_fully_adapted = true;
			}
			break;
		}
	}
	if (!has_mahoraga || !has_shrine){
		if (player) std::println("The World Cutting Slash cannot be unlocked, a core piece is missing!");
		return false;
	}
	if (!mahoraga_is_fully_adapted){
		if (player) std::println("The blueprint is incomplete. The World Cutting Slash cannot be used yet.");
		return false;
	}
	if (!tech->FullyChanted()){
		if (player) std::println("You need to chant fully to Pull off The World Cutting Slash!");
		return false;
	}
	return true;
}

void WorldCuttingSlash::UseSpecial(CurseUser* user, Character* target, Battlefield& bf){
	if (!CheckSpecial(user)) return;
	Character* tr = target;

	if (user->IsThePlayer()){
		tr = VList::TargetSelector(bf);
	}
	std::println(
		"{0}====  ======= =====  ===== =========   ====\n"
		"{0}=====  ======= =====  ===== =======   =====\n"
		"{0}========  =WORLD--CUTTING--SLASH==  =======\n"
		"{0}=========  ===== ====  ==== =====  ========\n"
		"{0}==========  ==== ====  ==== ====  ========={1}", 
		Color::Red, Color::Clear
	);
	tr->DamageBypassAll(damage);
	std::println("{} got hit by World Cutting Slash!", tr->GetNameWithID());
}

std::unique_ptr<Specials> WorldCuttingSlash::Clone() const {
	return std::make_unique<WorldCuttingSlash>(*this);
}

bool WorldCuttingSlash::IsWorldCuttingSlash() const {
	return true;
}