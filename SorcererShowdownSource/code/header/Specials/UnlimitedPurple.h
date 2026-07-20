#pragma once
#include "Specials/Specials.h"

class UnlimitedPurple : public Specials {
protected:
	static constexpr double uhp_damage = 1750.0;
public:
	UnlimitedPurple();
	std::unique_ptr<Specials> Clone() const override;
	bool UseSpecial(CurseUser* user, Character* target, Battlefield& bf) override;
	bool CheckSpecial(CurseUser* user) override;
	bool IsUnlimitedPurple() const override;
};