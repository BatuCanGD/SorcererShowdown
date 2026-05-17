#pragma once
#include "code/header/CursedTools/CursedTool.h"

class SplitSoulKatana : public CursedTool {
public:
	SplitSoulKatana();
	std::unique_ptr<CursedTool> Clone() const override;
	void UseTool(Character*, Character*) override;
};