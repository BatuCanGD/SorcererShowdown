#pragma once
#include "Characters/CurseUsers/CursedSpirits/CursedSpirit.h"

class Mahito : public CursedSpirit {
public:
	Mahito();
	std::unique_ptr<Character> Clone() const override;
	void OnCharacterTurn(Battlefield&) override;
};