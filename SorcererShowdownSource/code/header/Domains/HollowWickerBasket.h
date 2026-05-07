#pragma once
#include "code/header/Domains/Domain.h"

class CurseUser;
class Character;

class HollowWickerBasket : public Domain {
public:
	HollowWickerBasket();
	std::unique_ptr<Domain> Clone() const override;
};