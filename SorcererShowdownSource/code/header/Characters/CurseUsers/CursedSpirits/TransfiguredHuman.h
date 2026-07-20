#pragma once
#include "Characters/CurseUsers/CursedSpirits/CursedSpirit.h"

class TransfiguredHuman : public CursedSpirit {
public:
	TransfiguredHuman();
	void OnCharacterTurn(Battlefield&) override;
	bool IsTransfigured() const override;
};