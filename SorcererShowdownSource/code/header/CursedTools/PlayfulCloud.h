#pragma once
#include "code/header/CursedTools/CursedTool.h"

class PlayfulCloud final : public CursedTool {
public:
	PlayfulCloud();
	std::unique_ptr<CursedTool> Clone() const override;
	double GetCalculatedStrength(Character*) const override;
};