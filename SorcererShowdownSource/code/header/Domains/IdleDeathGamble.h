#pragma once
#include "code/header/Domains/Domain.h"

class CurseUser;
class Character;

class IdleDeathGamble : public Domain {
protected:
	double luck = 7.77;
	bool jackpot = false, text_dumped = false;
public:
	IdleDeathGamble();
	std::unique_ptr<Domain> Clone() const override;
	bool HasHitJackpot() const;
	void DumpInfo(Character*);
	void SetJackpot(bool);
	void OnSureHit(CurseUser& user, Character& target) override;
	bool IsIdleDeathGamble()const override;
};