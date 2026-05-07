#pragma once
#include "code/header/Specials/Specials.h"

class UnlimitedPurple : public Specials {
public:
	UnlimitedPurple();
	std::unique_ptr<Specials> Clone() const override;
	void PerformSpecial(CurseUser* user) override;
	virtual bool IsUnlimitedPurple() const;
};