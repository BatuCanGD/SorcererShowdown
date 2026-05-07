#pragma once
#include "code/header/Characters/PhysicallyGifted/PhysicallyGifted.h"

class Toji : public PhysicallyGifted {
public:
	Toji();
	std::unique_ptr<Character> Clone() const override;
	void OnCharacterTurn(Character*, Battlefield&) override;
};