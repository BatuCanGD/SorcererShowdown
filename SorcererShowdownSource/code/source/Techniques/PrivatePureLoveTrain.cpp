#include "code/header/Techniques/PrivatePureLoveTrain.h"
#include "code/header/Domains/IdleDeathGamble.h"
#include "code/header/Characters/Character.h"
#include "code/header/Characters/CurseUsers/CurseUser.h"
#include "code/header/GameManagement/Colors.h"
#include "code/header/GameManagement/Utils.h"

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
	int pplt = Utilities::GetRandomNumber(1, 239);
	if (pplt >= 200) {
		target->Damage(GOLDEN_PLINKO_DAMAGE);
		std::println("{} stuns {} with a Golden Plinko ball!", user->GetNameWithID(), target->GetNameWithID());
	}
	else if (pplt >= 100) {
		target->Damage(GREEN_PLINKO_DAMAGE);
		std::println("{} stuns {} with a Green Plinko ball!", user->GetNameWithID(), target->GetNameWithID());
	}
	else {
		target->Damage(RED_PLINKO_DAMAGE);
		std::println("{} stuns {} with a Red Plinko ball!", user->GetNameWithID(), target->GetNameWithID());
	}
	plinko_used = true;
	target->SetStunState(true);
}
void PrivatePureLoveTrain::UseShutterDoors(CurseUser* user, Character* target) {
	int pplt = Utilities::GetRandomNumber(1, 239);
	if (pplt >= 200) {
		target->Damage(GOLDEN_SHUTTER_DAMAGE);
		std::println("{} hits {} with a Golden Shutter!", user->GetNameWithID(), target->GetNameWithID());
	}
	else if (pplt >= 100) {
		target->Damage(GREEN_SHUTTER_DAMAGE);
		std::println("{} hits {} with a Green Shutter!", user->GetNameWithID(), target->GetNameWithID());
	}
	else {
		target->Damage(RED_SHUTTER_DAMAGE);
		std::println("{} hits {} with a Red Shutter!", user->GetNameWithID(), target->GetNameWithID());
	}
}
void PrivatePureLoveTrain::UseJackpotRush(CurseUser* user, Character* target) {
	double randomized_boost = Utilities::GetRandomNumber(5, 10) / 2.0;
	target->Damage(user->GetBaseAttackDamage() * randomized_boost);
	std::println("{} hits {} with a volley of jackpot boosted rush attacks!", user->GetNameWithID(), target->GetNameWithID());
}

void PrivatePureLoveTrain::TechniqueMenu(CurseUser* user, Character* target, Battlefield&) {
	if (user->DomainAmplificationActive()) {
		std::println("You cannot use your innate technique due to domain amplification!");
		return;
	}
	auto domain = user->GetDomain();
	if (domain && domain->IsIdleDeathGamble()) {
		auto idg = static_cast<IdleDeathGamble*>(domain);
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
	int choice = Utilities::GetInput<int>();

	switch (choice) {
	case 1:
		UsePlinkoBalls(user, target);
		break;
	case 2:
		UseShutterDoors(user, target);
		break;
	case 3:
		if (domain && domain->IsIdleDeathGamble()) {
			auto idg = static_cast<IdleDeathGamble*>(domain);
			if (!idg->HasHitJackpot()) {
				std::println("You arent able to use this");
				break;
			}
			UseJackpotRush(user, target);
		}
		else {
			std::println("Invalid Input");
		}
		break;
	default:
		std::println("Invalid Input");
	}
}

std::unique_ptr<Technique> PrivatePureLoveTrain::Clone() const {
	return std::make_unique<PrivatePureLoveTrain>(*this);
}

bool PrivatePureLoveTrain::AutoTechniqueUse(CurseUser* user, Character* target, Battlefield&) {
	if (Utilities::GetRandomNumber(1, 30) <= 25 && !plinko_used) {
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