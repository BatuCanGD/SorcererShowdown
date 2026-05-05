#pragma once
#include "Domain.h"

class CurseUser;
class Character;

class MalevolentShrine : public Domain {
public:
	MalevolentShrine();
	std::unique_ptr<Domain> Clone() const override;
	void OnSureHit(CurseUser& user, Character& target) override;
};