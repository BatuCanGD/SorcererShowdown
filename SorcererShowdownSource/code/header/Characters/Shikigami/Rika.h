#pragma once
#include "code/header/Characters/Shikigami/Shikigami.h"

class CurseUser;
struct Battlefield;

class Rika : public Shikigami {
private:
	bool value_saved = false;
	bool value_set = false;

	double user_ce = 0.0;
	double user_regen = 0.0;

	static constexpr double ce_mult = 10.0;
	static constexpr double regen_mult = 250.0;
	static constexpr int max_active_time = 5;
	int active_cooldown = 5;
public:
	Rika();
	void OnShikigamiTurn(CurseUser* user, Battlefield& bf) override;
	void SaveUserCursedEnergy(CurseUser*);
	void RikaCooldownRegeneration(CurseUser*);
};