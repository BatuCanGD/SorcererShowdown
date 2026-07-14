#pragma once
#include "code/header/Characters/Character.h"

struct Battlefield;
class CurseUser;

class Shikigami : public Character {
protected:
	int active_turn_amount = 0;
	enum class State {
		Disabled,
		Active
	};
	State shikigami_state = State::Disabled;
public:
	~Shikigami() override;
	Shikigami();
	bool CanBeAssignedID() const override;

	virtual void OnShikigamiTurn(CurseUser* user, Battlefield& bf) = 0;

	void Manifest();
	void Withdraw();
	void IncrementActiveTime();
	int GetActiveTime() const;
	bool IsActive() const;

	std::string GetShikigamiStatus() const;
	std::string GetType() const override;

	bool IsShikigami() const override;

	virtual bool IsMahoraga() const;
	virtual bool IsAgito() const;
};