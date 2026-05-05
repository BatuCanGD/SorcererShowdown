#pragma once
#include "CursedSpirit.h"

class Mahito : public CursedSpirit {
public:
	Mahito();
	std::unique_ptr<Character> Clone() const override;
	void OnCharacterTurn(Character*, Battlefield&) override;
};