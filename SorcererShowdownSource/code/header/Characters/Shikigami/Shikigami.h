#pragma once
#include "code/header/Characters/Character.h"

struct Battlefield;
class CurseUser;

class Shikigami : public Character {
protected:
	static constexpr double shadow_health_regen = 30.0;
	int active_turn_amount = 0;
	enum class State {
		Shadow,
		Partial,
		Full
	};
	State shikigami_state = State::Shadow;
public:
	virtual ~Shikigami();
	Shikigami();
	bool CanBeAssignedID() const override;

	virtual void OnShikigamiTurn(CurseUser* user, Battlefield& bf) = 0;

	void PartiallyManifest();
	void Manifest();
	void Withdraw();
	void ActiveTimeIncrementor();
	int GetActiveTime() const;
	bool IsActive() const;
	bool IsPartiallyActive() const;
	bool IsActivePhysically() const;	
	std::string GetShikigamiStatus() const;

	bool IsShikigami() const override;
	bool CanBeHit() const override;

	virtual bool IsMahoraga() const;
	virtual bool IsAgito() const;
};