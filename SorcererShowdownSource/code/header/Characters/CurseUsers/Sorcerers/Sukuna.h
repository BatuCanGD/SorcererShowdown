#pragma once
#include "code/header/Characters/CurseUsers/Sorcerers/Sorcerer.h"

class Sukuna : public Sorcerer {
public:
	Sukuna();
	std::unique_ptr<Character> Clone() const override;
	void OnCharacterTurn(Character*, Battlefield&) override;
};
