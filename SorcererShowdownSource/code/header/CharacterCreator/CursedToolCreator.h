#pragma once
#include "code/header/CharacterCreator/JSONLoader.h"
#include "code/header/CursedTools/CursedTool.h"

using json = nlohmann::json;
struct BattleCreator;

class CursedToolCreator : public JSONLoader<CursedTool> {
public:
	std::unique_ptr<CursedTool> CreateJsonObject(const json&, BattleCreator&) override;
	void LoadJsonObject(BattleCreator&) override;
};