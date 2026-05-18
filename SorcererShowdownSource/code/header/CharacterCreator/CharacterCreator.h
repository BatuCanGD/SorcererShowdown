#pragma once
#include "code/header/CharacterCreator/JSONLoader.h"
#include "code/header/Characters/Character.h"

using json = nlohmann::json;
struct BattleCreator;

class CharacterCreator : public JSONLoader<Character> {
public:
	std::unique_ptr<Character> CreateJsonObject(const json&) override;
	void LoadJsonObject(BattleCreator&) override;
};