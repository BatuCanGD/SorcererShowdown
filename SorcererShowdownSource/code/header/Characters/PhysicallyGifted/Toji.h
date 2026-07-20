#pragma once
#include "Characters/PhysicallyGifted/PhysicallyGifted.h"

class Toji : public PhysicallyGifted {
public:
	Toji();
	std::unique_ptr<Character> Clone() const override;
	void OnCharacterTurn(Battlefield&) override;
};