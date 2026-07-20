#pragma once
#include "CursedTools/CursedTool.h"

class SplitSoulKatana final : public CursedTool {
public:
	SplitSoulKatana();
	std::unique_ptr<CursedTool> Clone() const override;
};