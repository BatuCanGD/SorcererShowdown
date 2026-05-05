#pragma once
#include "CursedTool.h"

class Katana : public CursedTool {
public:
	Katana();
	std::unique_ptr<CursedTool> Clone() const override;
	void UseTool(Character*, Character*) override;
};