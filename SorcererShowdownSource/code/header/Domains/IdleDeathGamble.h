#pragma once
#include "Domains/Domain.h"

class CurseUser;
class Character;

class IdleDeathGamble final : public Domain {
protected:
	double luck = 7.77;
	int jackpot_tick = 0;
	bool jackpot = false, text_dumped = false;
	
	void DoSureHit(CurseUser& user, Character& target, bool is_blocked) override;
	void DumpInfo(Character*);
public:
	IdleDeathGamble();
	std::unique_ptr<Domain> Clone() const override;
	void TickDomainSpecialty(CurseUser*) override;
	bool HasHitJackpot() const;
	bool IsIdleDeathGamble()const override;
};