#pragma once
#include "code/header/Specials/Specials.h"

class WorldCuttingSlash : public Specials {
protected:
	static constexpr double wcs_damage = 2500.0;
public:
	WorldCuttingSlash();
	std::unique_ptr<Specials> Clone() const override;
	void UseSpecial(CurseUser* user, Character* target, Battlefield& bf) override;
	bool CheckSpecial(CurseUser* user) override;
	bool IsWorldCuttingSlash() const override;
};