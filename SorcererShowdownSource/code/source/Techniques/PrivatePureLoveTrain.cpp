#include "Techniques/PrivatePureLoveTrain.h"
#include "Domains/IdleDeathGamble.h"
#include "Characters/CurseUsers/CurseUser.h"
#include "GameManagement/Colors.h"
#include "GameManagement/Utils.h"

PrivatePureLoveTrain::PrivatePureLoveTrain() {
	name = "Private Pure Love Train";
	color = "\033[92m";
}

bool PrivatePureLoveTrain::PlinkoUsed() const {
	return plinko_used;
}
void PrivatePureLoveTrain::SetPlinkoStatus(bool b) {
	plinko_used = b;
}
void PrivatePureLoveTrain::TickPlinkoCooldown() {
	if (plinko_used) {
		plinko_cooldown_time++;
		if (plinko_cooldown_time >= max_plinko_cooldown_time) {
			plinko_used = false;
			plinko_cooldown_time = 0;
		}
	}
}

void PrivatePureLoveTrain::UsePlinkoBalls(CurseUser* user, Character* target) {
	if (plinko_used) return;
	int pplt = Utilities::GetRandom(1, 239);
	int idx{};
	if (pplt >= 200) {
		idx = 2;
		std::println("{} stuns {} with a Golden Plinko ball!", user->GetNameWithID(), target->GetNameWithID());
	}
	else if (pplt >= 100) {
		idx = 1;
		std::println("{} stuns {} with a Green Plinko ball!", user->GetNameWithID(), target->GetNameWithID());
	}
	else {
		std::println("{} stuns {} with a Red Plinko ball!", user->GetNameWithID(), target->GetNameWithID());
	}
	target->Damage(CalculateOutput(user, plinko_balls[idx]));
	plinko_used = true;
	target->SetStunState(true);
}
void PrivatePureLoveTrain::UseShutterDoors(CurseUser* user, Character* target) {
	int pplt = Utilities::GetRandom(1, 239);
	int idx{};
	if (pplt >= 200) {
		idx = 2;
		std::println("{} hits {} with a Golden Shutter!", user->GetNameWithID(), target->GetNameWithID());
	}
	else if (pplt >= 100) {
		idx = 1;
		std::println("{} hits {} with a Green Shutter!", user->GetNameWithID(), target->GetNameWithID());
	}
	else {
		std::println("{} hits {} with a Red Shutter!", user->GetNameWithID(), target->GetNameWithID());
	}
	target->Damage(CalculateOutput(user, shutter_doors[idx]));
}
void PrivatePureLoveTrain::UseJackpotRush(CurseUser* user, Character* target) {
	double randomized_boost = Utilities::GetRandom(5, 10) / 1.45;
	target->Damage(user->GetBaseAttackDamage() * randomized_boost);
	std::println("{} hits {} with a volley of jackpot boosted rush attacks!", user->GetNameWithID(), target->GetNameWithID());
}

bool PrivatePureLoveTrain::TechniqueMenu(CurseUser* user, Character* target, Battlefield&) {
	auto* domain = user->GetDomain();
	bool has_correct_domain = domain && domain->IsIdleDeathGamble();
	auto* idg = has_correct_domain ? static_cast<IdleDeathGamble*>(domain) : nullptr;

	if (has_correct_domain) {
		if (idg->HasHitJackpot()) {
			std::println("1 - Use Plinko balls | 2 - Use Shutter doors || 3 - {}Jackpot Rush{}", Color::Green, Color::Clear);
		}
		else {
			std::println("1 - Use Plinko balls | 2 - Use Shutter doors || 3 - {}Jackpot Rush (Unavailable){}",Color::DimGray, Color::Clear);
		}
	}
	else {
		std::println("1 - Use Plinko balls | 2 - Use Shutter doors");
	}
	std::print("=> ");
	
	switch (Utilities::GetInput<int>()) {
	case 1:
		UsePlinkoBalls(user, target);
		return true;
	case 2:
		UseShutterDoors(user, target);
		return true;
	case 3:
		if (has_correct_domain) {
			if (!idg->HasHitJackpot()) {
				std::println("You arent able to use this");
				return false;
			}
			UseJackpotRush(user, target);
			return true;
		}
		else {
			std::println("Invalid Input");
		}
		break;
	default:
		std::println("Invalid Input");
	}
	return false;
}

std::unique_ptr<Technique> PrivatePureLoveTrain::Clone() const {
	return std::make_unique<PrivatePureLoveTrain>(*this);
}

bool PrivatePureLoveTrain::AutoTechniqueUse(CurseUser* user, Character* target, Battlefield&) {
	if (Utilities::GetRandom(1, 30) <= 25 && !plinko_used) {
		UsePlinkoBalls(user, target);
		return true;
	}
	if (user->GetDomain() && user->GetDomain()->IsIdleDeathGamble()) {
		auto idg = static_cast<IdleDeathGamble*>(user->GetDomain());
		if (idg->HasHitJackpot()) {
			UseJackpotRush(user, target);
			return true;
		}
	}
	UseShutterDoors(user, target);
	return true;
}