#pragma once

struct Battlefield;
class CurseUser;

class Shikigami {
protected:
	std::string name{};
	std::string color{};

	int active_turn_amount = 0;
	enum class State {
		Disabled,
		Active
	};
	State shikigami_state = State::Disabled;
public:
	virtual ~Shikigami();
	Shikigami(std::string, std::string);

	virtual void OnShikigamiTurn(CurseUser* user, Battlefield& bf) = 0;
	std::string GetName() const;

	void Manifest();
	void Withdraw();
	void IncrementActiveTime();
	int GetActiveTime() const;
	bool IsActive() const;

	std::string GetShikigamiStatus() const;

	virtual bool IsMahoraga() const;
	virtual bool IsAgito() const;
};