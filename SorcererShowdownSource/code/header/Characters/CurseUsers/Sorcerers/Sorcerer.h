#pragma once
#include "code/header/Characters/CurseUsers/CurseUser.h"

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

	bool can_use_rct = true;
	enum class RCTProficiency {
		Crude,
		Adept,
		Expert,
		Absolute
	};
	RCTProficiency rct_skill = RCTProficiency::Adept;
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
	bool HasRCT() const;
	void UseRCT();

	double GetRCTHeal() const;
	double GetRCTCost() const;
	RCTProficiency GetRCTProficiency() const;
	void SetRCTUsability(bool);

	bool IsaSorcerer() const override;
	bool CanBeHit() const override;
};
