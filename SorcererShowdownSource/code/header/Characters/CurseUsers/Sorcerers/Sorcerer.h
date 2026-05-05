#pragma once
#include "CurseUser.h"

class Shikigami;
class Technique;
class Domain;
class Specials;

class Sorcerer : public CurseUser {
protected:
	bool six_eyes = false;
	enum class ReverseCT {
		Disabled, Active, Overdrive
	};
	ReverseCT rct_state = ReverseCT::Disabled;
	enum class RCTProficiency {
		None,
		Crude,
		Adept,
		Expert,
		Absolute
	};
	RCTProficiency rct_skill = RCTProficiency::None;
public:
	virtual ~Sorcerer();
	Sorcerer(double hp, double ce, double re);
	std::unique_ptr<Character> Clone() const override;
	
	void SetRCTProficiency(std::string);
	void SetSixEyes(bool);
	bool HasSixEyes() const;

	void SpendCE(double ce) override;

	void DisableRCT();
	void EnableRCT();
	void BoostRCT();
	std::string GetRCTstatus() const;
	void UseRCT();

	double GetRCTHeal() const;
	double GetRCTCost() const;
	RCTProficiency GetRCTProficiency() const;

	bool IsaSorcerer() const override;
	bool CanBeHit() const override;
};
