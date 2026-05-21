#include "code/header/CharacterCreator/CursedToolCreator.h"
#include "code/header/GameManagement/Utils.h"
#include "code/header/GameManagement/UserInterface.h"
#include "code/header/GameManagement/BattlefieldHeader.h"
#include "code/header/Characters/CurseUsers/CurseUser.h"

std::unique_ptr<CursedTool> CursedToolCreator::CreateJsonObject(const json& j, BattleCreator&) {
    auto cursedtool = std::make_unique<CursedTool>();
    if (j.contains("name")){
        cursedtool->SetName(j.at("name").get<std::string>());
    }
    if (j.contains("color")){
        cursedtool->SetColor(j.at("color").get<std::string>());
    }
    if (j.contains("type")){
        cursedtool->SetType(j.at("type").get<std::string>());
    }
    if (j.contains("damage")){
        cursedtool->SetDamage(j.at("damage").get<double>());
    }
    return cursedtool;
}
void CursedToolCreator::LoadJsonObject(BattleCreator& bc) {
    std::println(stderr, "Looking for cursedtools.json in: {}", std::filesystem::current_path().string());
    std::ifstream file("cursedtools.json");

    if (!file.is_open()) {
        std::println(stderr, "Could not find cursedtools.json!");
        Utilities::WaitForInput();
        return;
    }
    nlohmann::json data;
    try {
        file >> data;
    }
    catch (const nlohmann::json::parse_error& e) {
        std::println(stderr, "JSON Parse Error: {}", e.what());
        Utilities::WaitForInput();
        return;
    }

    if (data.contains("cursedtools") && data["cursedtools"].is_array()) {
        for (const auto& domainData : data["cursedtools"]) {
            std::unique_ptr<CursedTool> newTool = CursedToolCreator::CreateJsonObject(domainData, bc);
            if (newTool) {
                bc.cursedtoollist.push_back(std::move(newTool));
            }
        }
        UserInterface::ClearScreen();
    }
}