#pragma once
#include "json.hpp"
#include "code/header/Characters/Character.h"

using json = nlohmann::json;
struct BattleCreator;

class CharacterCreator {
public:
	static std::unique_ptr<Character> CreateFromJson(const json&);
	void LoadJsonCharacter(BattleCreator&);
};