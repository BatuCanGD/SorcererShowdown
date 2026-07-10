#pragma once
#include "code/header/CursedTools/CursedTool.h"

class Katana final : public CursedTool {
public:
	Katana();
	std::unique_ptr<CursedTool> Clone() const override;
};