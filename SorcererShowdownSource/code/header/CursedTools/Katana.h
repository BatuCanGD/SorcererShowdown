#pragma once
#include "code/header/CursedTools/CursedTool.h"

class Katana : public CursedTool {
public:
	Katana();
	std::unique_ptr<CursedTool> Clone() const override;
};