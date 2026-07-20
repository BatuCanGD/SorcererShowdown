#pragma once
#include "Characters/Shikigami/Shikigami.h"

class CurseUser;
struct Battlefield;

class Agito : public Shikigami {
protected:
	static constexpr double passive_heal_amount = 50.0;
	static constexpr double summon_amount = 80.0;
public:
	Agito();
	void PassiveSupport(CurseUser* user);
	void OnShikigamiTurn(CurseUser* user, Battlefield& bf) override;
	bool IsAgito() const override;
};