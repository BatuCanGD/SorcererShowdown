#pragma once
#include "code/header/Characters/CurseUsers/Sorcerers/Sorcerer.h"

class Yuta : public Sorcerer {
public:
	Yuta();
	std::unique_ptr<Character> Clone() const override;
	void OnCharacterTurn(Character*, Battlefield&) override;
	bool InfCheck(Character*);
};