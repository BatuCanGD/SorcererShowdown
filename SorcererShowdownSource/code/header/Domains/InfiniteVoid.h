#pragma once
#include "code/header/Domains/Domain.h"

class CurseUser;
class Character;

class InfiniteVoid : public Domain {
	void DoSureHit(CurseUser& user, Character& target, bool is_blocked) override;
public:
	InfiniteVoid();
	std::unique_ptr<Domain> Clone() const override;
};