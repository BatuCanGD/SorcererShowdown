#pragma once
#include "code/header/CursedTools/CursedTool.h"

class InvertedSpearofHeaven : public CursedTool {
private:
	double special_tool_damage = base_tool_damage * 2.25;
public:
	InvertedSpearofHeaven();
	std::unique_ptr<CursedTool> Clone() const override;
	void UseTool(Character*, Character*) override;
	bool IsAntiTechniqueWeapon() const override;
};