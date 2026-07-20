#pragma once
#include "Characters/CurseUsers/Sorcerers/Sorcerer.h"

class Yuta : public Sorcerer {
public:
	Yuta();
	std::unique_ptr<Character> Clone() const override;
	void OnCharacterTurn(Battlefield&) override;
};