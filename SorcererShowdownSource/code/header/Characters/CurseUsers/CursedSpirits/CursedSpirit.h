#pragma once
#include "code/header/Characters/CurseUsers/CurseUser.h"

class CursedSpirit : public CurseUser {
protected:
	double passive_health_regen = 5.0;
public:
	CursedSpirit(double hp, double ce, double regen);
	~CursedSpirit() override;
	std::unique_ptr<Character> Clone() const override;
	bool IsaCursedSpirit() const override;
	void TickCharacterSpecialty() override;
	void SetPassiveRegen(double);
	virtual bool IsTransfigured() const;
};