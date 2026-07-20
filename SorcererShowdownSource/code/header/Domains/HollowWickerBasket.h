#pragma once
#include "Domains/Domain.h"

class CurseUser;
class Character;

class HollowWickerBasket final : public Domain {
public:
	HollowWickerBasket();
	std::unique_ptr<Domain> Clone() const override;
};