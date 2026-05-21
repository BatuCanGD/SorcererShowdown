#include "code/header/CharacterCreator/DomainCreator.h"
#include "code/header/GameManagement/Utils.h"
#include "code/header/GameManagement/UserInterface.h"
#include "code/header/GameManagement/BattlefieldHeader.h"
#include "code/header/Characters/CurseUsers/CurseUser.h"

std::unique_ptr<Domain> DomainCreator::CreateJsonObject(const json& j, BattleCreator&) {
    std::unique_ptr<Domain> domain;

    if (j.contains("is_neutralizer") && j.at("is_neutralizer").get<bool>()) {
        domain = std::make_unique<Domain>(1.0, 1.0, 1.0);
        domain->SetIfDomainNeutralizer(true);
    } else {
        if (!j.contains("health") || !j.contains("strength") || !j.contains("range")) {
            return nullptr; 
        }
        domain = std::make_unique<Domain>(
            j.at("health").get<double>(),
            j.at("strength").get<double>(),
            j.at("range").get<double>()
        );
    }
    if (j.contains("domain_type")) {
        domain->SetDomainType(j.at("domain_type").get<std::string>());
    }
    if (j.contains("refinement")) {
        domain->SetRefinement(j.at("refinement").get<std::string>());
    }
    if (j.contains("name")) {
        domain->SetDomainName(j.at("name").get<std::string>());
    }
    if (j.contains("color")) {
        domain->SetDomainColor(j.at("color").get<std::string>()); 
    }
    if (j.contains("cost")) {
        domain->SetDomainCost(j.at("cost").get<double>()); 
    }
    if (j.contains("can_stun")) {
        domain->SetDomainStun(j.at("can_stun").get<bool>());
    }
    if (j.contains("surehit_damage")) {
        domain->SetSurehitDamage(j.at("surehit_damage").get<double>());
    }
    return domain;
}
void DomainCreator::LoadJsonObject(BattleCreator& bc) {
    std::println(stderr, "Looking for domains.json in: {}", std::filesystem::current_path().string());
    std::ifstream file("domains.json");

    if (!file.is_open()) {
        std::println(stderr, "Could not find domains.json!");
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

    if (data.contains("domains") && data["domains"].is_array()) {
        for (const auto& domainData : data["domains"]) {
            std::unique_ptr<Domain> newDomain = DomainCreator::CreateJsonObject(domainData, bc);
            if (newDomain) {
                bc.domainlist.push_back(std::move(newDomain));
            }
        }
        UserInterface::ClearScreen();
    }
}