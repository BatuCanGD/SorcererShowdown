#pragma once
#include "code/header/Characters/Shikigami/Shikigami.h"

class CurseUser;
struct Battlefield;

class Agito : public Shikigami {
protected:
	const double passive_heal_amount = 50.0;
	const double summon_amount = 80.0;
public:
	Agito();
	void PassiveSupport(CurseUser* user);
	void OnShikigamiTurn(CurseUser* user, Battlefield& bf) override;
	bool IsAgito() const override;
};