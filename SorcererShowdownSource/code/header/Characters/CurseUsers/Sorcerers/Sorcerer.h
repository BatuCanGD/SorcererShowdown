#pragma once
#include "code/header/Characters/CurseUsers/CurseUser.h"

class Shikigami;
class Technique;
class Domain;
class Specials;

class Sorcerer : public CurseUser {
protected:
	double rct_amount{};

	enum class RCTProficiency { Wasteful, Crude, Adept, Expert, Absolute };
	RCTProficiency rct_skill = RCTProficiency::Adept;

	bool six_eyes{};
	bool can_use_rct{true};
public:
	~Sorcerer() override;
	Sorcerer(double hp, double ce, double re);
	std::unique_ptr<Character> Clone() const override;
	
	void SpendCE(double ce) override;
	void SetRCTAmount(double heal_amount);

	std::string GetType() const override;
	std::string GetRCTstatus() const;

	void TickRCT();

	double GetRCTCost() const;
	RCTProficiency GetRCTProficiency() const;

	bool HasRCT() const;
	bool HasSixEyes() const;
	bool IsaSorcerer() const override;

	void SetRCTProficiency(std::string);
	void SetSixEyes(bool);
	void SetRCTUsability(bool);
};
