#pragma once
#include "code/header/Domains/Domain.h"

class CurseUser;
class Character;

class AuthenticMutualLove : public Domain {
	void DoSureHit(CurseUser& user, Character& target, bool is_blocked) override;
public:
	AuthenticMutualLove();
	std::unique_ptr<Domain> Clone() const override;
};