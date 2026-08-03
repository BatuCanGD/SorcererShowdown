#pragma once
#include "CharacterCreator/JSONLoader.h"
#include "Characters/Character.h"

struct BattleCreator;
class CharacterCreator final : public JSONLoader<Character> {
public:
	std::unique_ptr<Character> CreateJsonObject(const nlohmann::json&, BattleCreator&) override;
	void LoadJsonObject(BattleCreator&) override;
};