#pragma once
#include "Domains/Domain.h"

class CurseUser;
class Character;

class InfiniteVoid final : public Domain {
	void DoSureHit(CurseUser& user, Character& target, bool is_blocked) override;
public:
	InfiniteVoid();
	std::unique_ptr<Domain> Clone() const override;
};