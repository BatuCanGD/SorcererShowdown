#pragma once
#include "Specials.h"

class WorldCuttingSlash : public Specials {
public:
	WorldCuttingSlash();
	std::unique_ptr<Specials> Clone() const override;
	void PerformSpecial(CurseUser* user) override;
	virtual bool IsWorldCuttingSlash() const;
};