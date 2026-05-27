#pragma once
#include "code/header/Characters/Character.h"

class Dummy : public Character {
protected:
    unsigned int i = 0;
public:
	Dummy();
	std::unique_ptr<Character> Clone() const override;
	void OnCharacterTurn(Battlefield&) override;
};