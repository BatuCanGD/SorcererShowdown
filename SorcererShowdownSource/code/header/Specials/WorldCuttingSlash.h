#pragma once
#include "code/header/Specials/Specials.h"

class WorldCuttingSlash : public Specials {
public:
	WorldCuttingSlash();
	std::unique_ptr<Specials> Clone() const override;
	void PerformSpecial(CurseUser* user) override;
	bool IsWorldCuttingSlash() const override;
};