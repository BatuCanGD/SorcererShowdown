#pragma once
#include "code/header/Domains/Domain.h"

class CurseUser;
class Character;

class MalevolentShrine : public Domain {
	void DoSureHit(CurseUser& user, Character& target, bool is_blocked) override;
public:
	MalevolentShrine();
	std::unique_ptr<Domain> Clone() const override;
};