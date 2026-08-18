#pragma once
#include "Characters/CurseUsers/Sorcerers/Sorcerer.h"

class Hakari : public Sorcerer {
public:
	Hakari();
	std::unique_ptr<Character> Clone() const override;
	void OnCharacterTurn(Battlefield&) override;
};