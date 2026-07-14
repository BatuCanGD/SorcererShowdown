#include "code/header/Characters/Shikigami/Mahoraga.h"
#include "code/header/Techniques/Shrine.h"
#include "code/header/GameManagement/VList.h"
#include "code/header/Characters/CurseUsers/Sorcerers/Sorcerer.h"
#include "code/header/Specials/WorldCuttingSlash.h"
#include "code/header/GameManagement/Colors.h"



WorldCuttingSlash::WorldCuttingSlash() {
	name = "World Cutting Slash";
	color = "\033[31m";
}

bool WorldCuttingSlash::CheckSpecial(CurseUser* user) {
	auto* tech = user->GetTechnique();
	bool has_shrine = tech && tech->IsShrine();
	bool player = user->IsThePlayer();
	
	if (!has_shrine){
		if (player) std::println("You lack the Technique to use the special with!");
		return false;
	}
	bool has_mahoraga = false, mahoraga_is_fully_adapted = false;
	for (const auto& s : user->GetShikigami()) {
		if (s->IsMahoraga()) {
			has_mahoraga = true;
			mahoraga_is_fully_adapted = static_cast<Mahoraga*>(s.get())->FullyAdapted();
			break;
		}
	}
	if (!has_mahoraga){
		if (player) std::println("The World Cutting Slash cannot be unlocked, a core piece is missing!");
		return false;
	}
	if (!mahoraga_is_fully_adapted){
		if (player) std::println("The blueprint is incomplete. The World Cutting Slash cannot be used yet.");
		return false;
	}
	if (!tech->FullyChanted()){
		if (player) std::println("You need to chant fully to pull off The World Cutting Slash!");
		return false;
	}
	return true;
}

void WorldCuttingSlash::UseSpecial(CurseUser* user, Character* target, Battlefield& bf){
	if (!CheckSpecial(user)) return;

	if (user->IsThePlayer()) target = VList::TargetSelector(bf, user); 
	else if(!target) target = VList::TargetChooser(user, bf);

	std::println(
		"{0}==  ======== =======  ======= ========  ==\n"
		"{0}===  ======== ======  ====== ========  ===\n"
		"{0}====  ======== =====  ===== ========  ====\n"
		"{0}=====  ======= =====  ===== =======  =====\n"
		"{0}========  =WORLD--CUTTING--SLASH=  =======\n"
		"{0}=========  ===== ===  ===== ====  ========\n"
		"{0}===========  === ===  ==== ===  ==========\n"
		"{0}============  === ==  === ===  ===========\n" 
		"{0}=============  === =  == ===  ==========={1}", 
		Color::Red, Color::Clear
	);
	if (!target){
		std::println("The World Cutting Slash missed!");
	}else{
		target->DamageBypassAll(wcs_damage);
		std::println("{} got hit by World Cutting Slash!", target->GetNameWithID());
	}
}

std::unique_ptr<Specials> WorldCuttingSlash::Clone() const {
	return std::make_unique<WorldCuttingSlash>(*this);
}

bool WorldCuttingSlash::IsWorldCuttingSlash() const {
	return true;
}