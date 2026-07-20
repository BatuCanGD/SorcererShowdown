#pragma once
#include <nlohmann/json.hpp>

struct BattleCreator;
template<typename T>
class JSONLoader {
public:
    virtual ~JSONLoader() = default;
    virtual std::unique_ptr<T> CreateJsonObject(const nlohmann::json&, BattleCreator&) = 0;
    virtual void LoadJsonObject(BattleCreator&) = 0;
};

namespace Jtils {
    inline nlohmann::json GetSection(const nlohmann::json& j, std::string_view name) {
        return (j.contains(name) && j.at(name).is_object()) ? j.at(name) : j;
    }
}