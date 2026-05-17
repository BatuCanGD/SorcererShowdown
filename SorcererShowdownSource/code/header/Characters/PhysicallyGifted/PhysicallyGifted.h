#pragma once
#include "code/header/Characters/Character.h"

class PhysicallyGifted : public Character {
protected:
	double strength;
	double strength_to_reinforcement_ratio = 500.0;
	double strength_to_damage_ratio = 10.0;
public:
	PhysicallyGifted(double hp, double str);
	virtual ~PhysicallyGifted() = default;
	std::unique_ptr<Character> Clone() const override;

	void Attack(Character*) override;
	void TickCharacterSpecialty() override;

	void SetStrength(double);
	double GetStrength() const;
	double GetStrengthDamage()const;
	double GetDamageReinforcement()const override;
	double GetHealingFactor()const;
	bool IsPhysicallyGifted() const override;
};