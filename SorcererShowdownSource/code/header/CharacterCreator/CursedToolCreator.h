#pragma once
#include "CharacterCreator/JSONLoader.h"
#include "CursedTools/CursedTool.h"

struct BattleCreator;
class CursedToolCreator : public JSONLoader<CursedTool> {
public:
	std::unique_ptr<CursedTool> CreateJsonObject(const nlohmann::json&, BattleCreator&) override;
	void LoadJsonObject(BattleCreator&) override;
};