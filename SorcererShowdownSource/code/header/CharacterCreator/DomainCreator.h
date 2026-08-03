#pragma once
#include "CharacterCreator/JSONLoader.h"
#include "Domains/Domain.h"

struct BattleCreator;
class DomainCreator final : public JSONLoader<Domain> {
public:
	std::unique_ptr<Domain> CreateJsonObject(const nlohmann::json&, BattleCreator&) override;
	void LoadJsonObject(BattleCreator&) override;
};