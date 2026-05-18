#pragma once
#include "code/header/CursedTools/CursedTool.h"

class InvertedSpearofHeaven : public CursedTool {
public:
	InvertedSpearofHeaven();
	std::unique_ptr<CursedTool> Clone() const override;
	void UseTool(Character*, Character*) override;
	bool IsAntiTechniqueWeapon() const override;
};