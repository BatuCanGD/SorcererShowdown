#pragma once
#include "CursedSpirit.h"

class TransfiguredHuman : public CursedSpirit {
public:
	TransfiguredHuman();
	void OnCharacterTurn(Character*, Battlefield&) override;
	bool IsTransfigured() const override;
};