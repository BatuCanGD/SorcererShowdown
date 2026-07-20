#pragma once
#include "Domains/Domain.h"

class CurseUser;
class Character;

class SimpleDomain final : public Domain {
public:
	SimpleDomain();
	std::unique_ptr<Domain> Clone() const override;
};