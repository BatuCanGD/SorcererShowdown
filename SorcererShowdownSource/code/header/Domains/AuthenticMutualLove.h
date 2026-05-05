#pragma once
#include "Domain.h"

class CurseUser;
class Character;

class AuthenticMutualLove : public Domain {
public:
	AuthenticMutualLove();
	std::unique_ptr<Domain> Clone() const override;
	void OnSureHit(CurseUser& user, Character& target) override;
};