#pragma once
#include "code/header/Characters/CurseUsers/Sorcerers/Sorcerer.h"

class Hakari : public Sorcerer {
protected:
	int jackpot_tick = 0;
public:
	Hakari();
	std::unique_ptr<Character> Clone() const override;
	void OnCharacterTurn(Character*, Battlefield&) override;
	void TickCharacterSpecialty() override;
};