#include "code/header/CharacterCreator/DomainCreator.h"
#include "code/header/GameManagement/Utils.h"
#include "code/header/GameManagement/UserInterface.h"
#include "code/header/GameManagement/BattlefieldHeader.h"
#include "code/header/Characters/CurseUsers/CurseUser.h"

std::unique_ptr<Domain> DomainCreator::CreateJsonObject(const json& j, BattleCreator&) {
    std::unique_ptr<Domain> domain;
    const json& id = (j.contains("identity") && j.at("identity").is_object()) ? j.at("identity") : j;
    const json& st = (j.contains("stats") && j.at("stats").is_object()) ? j.at("stats") : j;
    const json& cf = (j.contains("config") && j.at("config").is_object()) ? j.at("config") : j;

    if (cf.contains("is_neutralizer") && cf.at("is_neutralizer").get<bool>()) {
        domain = std::make_unique<Domain>(1.0, 1.0, 1);
        domain->SetIfDomainNeutralizer(true);
    } else {
        domain = std::make_unique<Domain>(
            st.at("health").get<double>(),
            st.at("strength").get<double>(),
            st.at("range").get<int>()
        );
    }
    if (!domain) return nullptr;

    if (id.contains("name")) {
        domain->SetDomainName(id.at("name").get<std::string>());
    }
    if (id.contains("color")) {
        domain->SetDomainColor(id.at("color").get<std::string>()); 
    }
    if (cf.contains("domain_type")) {
        domain->SetDomainType(cf.at("domain_type").get<std::string>());
    }
    if (cf.contains("refinement")) {
        domain->SetRefinement(cf.at("refinement").get<std::string>());
    }
    if (cf.contains("cost")) {
        domain->SetDomainCost(cf.at("cost").get<double>()); 
    }
    if (cf.contains("can_stun")) {
        domain->SetDomainStun(cf.at("can_stun").get<bool>());
    }
    if (cf.contains("surehit_damage")) {
        domain->SetSurehitDamage(cf.at("surehit_damage").get<double>());
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