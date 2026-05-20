#pragma once
#include "code/header/CharacterCreator/JSONLoader.h"
#include "code/header/Domains/Domain.h"

using json = nlohmann::json;
struct BattleCreator;

class DomainCreator : public JSONLoader<Domain> {
public:
	std::unique_ptr<Domain> CreateJsonObject(const json&, BattleCreator&) override;
	void LoadJsonObject(BattleCreator&) override;
};