#pragma once
#include "json.hpp"

using json = nlohmann::json;
struct BattleCreator;

template<typename T>
class JSONLoader {
public:
    virtual ~JSONLoader() = default;
    virtual std::unique_ptr<T> CreateJsonObject(const json&, BattleCreator&) = 0;
    virtual void LoadJsonObject(BattleCreator&) = 0;
};