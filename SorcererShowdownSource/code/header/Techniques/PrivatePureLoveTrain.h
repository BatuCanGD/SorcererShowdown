#pragma once
#include "Techniques.h"

class CurseUser;
class Character;
struct Battlefield;

class PrivatePureLoveTrain : public Technique {
protected:
	const double GOLDEN_SHUTTER_DAMAGE = 90.0;
	const double RED_SHUTTER_DAMAGE = 25.0;
	const double GREEN_SHUTTER_DAMAGE = 55.0;	

	const double GOLDEN_PLINKO_DAMAGE = 65.0;
	const double RED_PLINKO_DAMAGE = 10.0;
	const double GREEN_PLINKO_DAMAGE = 35.0;

	int plinko_cooldown_time = 0;
	const int max_plinko_cooldown_time = 2;
	bool plinko_used = false;
public:
	PrivatePureLoveTrain();

	bool PlinkoUsed() const;
	void SetPlinkoStatus(bool);
	void TickPlinkoCooldown();

	void UsePlinkoBalls(CurseUser* user, Character* target);
	void UseShutterDoors(CurseUser* user, Character* target);
	void UseJackpotRush(CurseUser* user, Character* target);

	void TechniqueMenu(CurseUser* user, Character* target, Battlefield&) override;
	void AutoTechniqueUse(CurseUser* user, Character* target, Battlefield& bf) override;
	std::unique_ptr<Technique> Clone() const override;
};