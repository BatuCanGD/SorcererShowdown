#pragma once
#include "Domains/Domain.h"

class CurseUser;
class Character;

class MalevolentShrine final : public Domain {
public:
	MalevolentShrine();
	std::unique_ptr<Domain> Clone() const override;
};