#pragma once
#include "Domain.h"

class CurseUser;
class Character;

class InfiniteVoid : public Domain {
public:
	InfiniteVoid();
	std::unique_ptr<Domain> Clone() const override;
	void OnSureHit(CurseUser& user, Character& target) override;
};