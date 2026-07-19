#pragma once
#include "Techniques.h"

class CurseUser;
class Character;
struct Battlefield;

class PrivatePureLoveTrain : public Technique {
protected:
	static constexpr double plinko_balls[3] = { 40.0, 80.0, 145.0 };
	static constexpr double shutter_doors[3] = { 75.0, 130.0, 195.0 };

	int plinko_cooldown_time = 0;
	static constexpr int max_plinko_cooldown_time = 2;
	bool plinko_used = false;
public:
	PrivatePureLoveTrain();

	bool PlinkoUsed() const;
	void SetPlinkoStatus(bool);
	void TickPlinkoCooldown();

	void UsePlinkoBalls(CurseUser* user, Character* target);
	void UseShutterDoors(CurseUser* user, Character* target);
	void UseJackpotRush(CurseUser* user, Character* target);

	bool TechniqueMenu(CurseUser* user, Character* target, Battlefield&) override;
	bool AutoTechniqueUse(CurseUser* user, Character* target, Battlefield& bf) override;
	std::unique_ptr<Technique> Clone() const override;
};