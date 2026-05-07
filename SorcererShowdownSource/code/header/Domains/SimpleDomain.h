#pragma once
#include "code/header/Domains/Domain.h"

class CurseUser;
class Character;

class SimpleDomain : public Domain {
public:
	SimpleDomain();
	std::unique_ptr<Domain> Clone() const override;
};