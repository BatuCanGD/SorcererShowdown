#pragma once
#include "Characters/CurseUsers/Sorcerers/Sorcerer.h"

class Gojo : public Sorcerer {
public:
	Gojo();
	std::unique_ptr<Character> Clone() const override;
	void OnCharacterTurn(Battlefield&) override;
};