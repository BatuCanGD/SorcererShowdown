#pragma once
#include "CursedTool.h"

class PlayfulCloud : public CursedTool {
public:
	PlayfulCloud();
	std::unique_ptr<CursedTool> Clone() const override;
	void UseTool(Character*, Character*) override;
};