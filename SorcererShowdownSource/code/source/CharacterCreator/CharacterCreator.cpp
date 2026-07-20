#include "CharacterCreator/CharacterCreator.h"
#include "CharacterCreator/AI/CharacterAI.h"
#include "CharacterCreator/AI/Aggressive.h"
#include "CharacterCreator/AI/Reactive.h"
#include "CharacterCreator/AI/Brawler.h"
#include "CharacterCreator/AI/Randomized.h"

#include "GameManagement/Utils.h"
#include "GameManagement/BattlefieldHeader.h"

#include "Characters/Shikigami/Rika.h"
#include "Characters/CurseUsers/CurseUser.h"
#include "Characters/CurseUsers/Sorcerers/Sorcerer.h"
#include "Characters/CurseUsers/CursedSpirits/CursedSpirit.h"
#include "Characters/PhysicallyGifted/PhysicallyGifted.h"
#include "Characters/Shikigami/Mahoraga.h"
#include "Characters/Shikigami/Agito.h"

#include "Techniques/Limitless.h"
#include "Techniques/Shrine.h"
#include "Techniques/IdleTransfiguration.h"
#include "Techniques/PrivatePureLoveTrain.h"
#include "Techniques/Copy.h"

#include "Domains/MalevolentShrine.h"
#include "Domains/AuthenticMutualLove.h"
#include "Domains/IdleDeathGamble.h"
#include "Domains/SimpleDomain.h"
#include "Domains/HollowWickerBasket.h"
#include "Domains/SelfEmbodimentOfPerfection.h"
#include "Domains/InfiniteVoid.h"

#include "CursedTools/InvertedSpearOfHeaven.h"
#include "CursedTools/Katana.h"
#include "CursedTools/PlayfulCloud.h"
#include "CursedTools/SplitSoulKatana.h"

#include "Specials/UnlimitedPurple.h"
#include "Specials/WorldCuttingSlash.h"

#include "GameManagement/UserInterface.h"

static std::unique_ptr<Technique> GetTechniqueByName(std::string_view name) {
    static const std::unordered_map<std::string, std::function<std::unique_ptr<Technique>()>> techniques = {
        {"Limitless", []() { return std::make_unique<Limitless>(); }},
        {"Shrine", []() { return std::make_unique<Shrine>(); }},
        {"Copy", []() { return std::make_unique<Copy>(); }},
        {"Idle Transfiguration", []() { return std::make_unique<IdleTransfiguration>(); }},
        {"Private Pure Love Train", []() { return std::make_unique<PrivatePureLoveTrain>(); }}
    };
    auto it = techniques.find(std::string(name));
    return (it != techniques.end()) ? it->second() : nullptr;
}

static std::unique_ptr<CharacterBrain> GetBrainType(std::string_view name) {
    static const std::unordered_map<std::string, std::function<std::unique_ptr<CharacterBrain>()>> brains = {
        {"Aggressive", []() { return std::make_unique<Aggressive>(); }},
        {"Reactive", []() { return std::make_unique<Reactive>(); }},
        {"Randomized", []() { return std::make_unique<Randomized>(); }},
        {"Brawler", []() { return std::make_unique<Brawler>(); }}
    };
    auto it = brains.find(std::string(name));
    return (it != brains.end()) ? it->second() : std::make_unique<Aggressive>();
}

static std::unique_ptr<Domain> GetDomainByName(std::string_view name, BattleCreator& bc) {
    static const std::unordered_map<std::string, std::function<std::unique_ptr<Domain>()>> domains = {
        {"Infinite Void", []() { return std::make_unique<InfiniteVoid>(); }},
        {"Malevolent Shrine", []() { return std::make_unique<MalevolentShrine>(); }},
        {"Authentic Mutual Love", []() { return std::make_unique<AuthenticMutualLove>(); }},
        {"Idle Death Gamble", []() { return std::make_unique<IdleDeathGamble>(); }},
        {"Self Embodiment of Perfection", []() { return std::make_unique<SelfEmbodimentOfPerfection>(); }}
    };
    auto it = domains.find(std::string(name));
    if (it != domains.end()) {
        return it->second();
    }
    for (const auto& domain : bc.domainlist) {
        if (domain->GetDomainSimpleName() == name) {
            return domain->Clone();
        }
    }
    return nullptr;
}
static std::unique_ptr<Domain> GetCounterDomainByName(std::string_view name, BattleCreator& bc) {
static const std::unordered_map<std::string, std::function<std::unique_ptr<Domain>()>> counters = {
        {"Simple Domain", []() { return std::make_unique<SimpleDomain>(); }},
        {"Hollow Wicker Basket", []() { return std::make_unique<HollowWickerBasket>(); }}
    };
    auto it = counters.find(std::string(name));
    if (it != counters.end()) {
        return it->second();
    }
    for (const auto& domain : bc.domainlist) {
        if (domain->IsNeutralizer() && domain->GetDomainSimpleName() == name) {
            return domain->Clone();
        }
    }
    return nullptr;
}

static std::unique_ptr<Specials> GetSpecialByName(std::string_view name) {
    static const std::unordered_map<std::string, std::function<std::unique_ptr<Specials>()>> specialz = {
        {"Unlimited Purple", []() { return std::make_unique<UnlimitedPurple>(); }},
        {"World Cutting Slash", []() { return std::make_unique<WorldCuttingSlash>(); }}
    };
    auto it = specialz.find(std::string(name));
    return (it != specialz.end()) ? it->second() : nullptr;
}

static std::unique_ptr<CursedTool> GetToolByName(std::string_view name, BattleCreator& bc) {
    static const std::unordered_map<std::string, std::function<std::unique_ptr<CursedTool>()>> tools = {
        {"The Inverted Spear of Heaven", []() { return std::make_unique<InvertedSpearofHeaven>(); }},
        {"Playful Cloud", []() { return std::make_unique<PlayfulCloud>(); }},
        {"Split Soul Katana", []() { return std::make_unique<SplitSoulKatana>(); }},
        {"Katana", []() { return std::make_unique<Katana>(); }}
    };
    auto it = tools.find(std::string(name));
    if (it != tools.end()) {
        return it->second();
    }
    for (const auto& tool : bc.cursedtoollist) {
        if (tool->GetSimpleName() == name) {
            return tool->Clone();
        }
    }
    return nullptr;
}

static std::unique_ptr<Shikigami> GetShikigamiByName(std::string_view name) {
    static const std::unordered_map<std::string, std::function<std::unique_ptr<Shikigami>()>> shikigami = {
        {"Agito", []() { return std::make_unique<Agito>(); }},
        {"Mahoraga", []() { return std::make_unique<Mahoraga>(); }},
        {"Rika", []() { return std::make_unique<Rika>(); }}
    };
    auto it = shikigami.find(std::string(name));
    return (it != shikigami.end()) ? it->second() : nullptr;
}

std::unique_ptr<Character> CharacterCreator::CreateJsonObject(const nlohmann::json& j, BattleCreator& bc) {
    std::string type = j.at("type").get<std::string>();
    std::unique_ptr<Character> character = nullptr;

    auto id = Jtils::GetSection(j, "identity");
    auto st = Jtils::GetSection(j, "stats");
    auto cf = Jtils::GetSection(j, "config");
    auto sr = Jtils::GetSection(j, "sorcery");
    auto kt = Jtils::GetSection(sr, "kit");
    auto tn = Jtils::GetSection(sr, "tuning");
    auto tl = Jtils::GetSection(j, "tools");
    
    if (type == "Sorcerer") {
        auto s = std::make_unique<Sorcerer>(
            st.at("hp").get<double>(),
            st.at("ce").get<double>(),
            st.at("regen").get<double>());
        if (cf.contains("six_eyes")) s->SetSixEyes(cf.at("six_eyes").get<bool>());
        if (cf.contains("can_use_rct")) s->SetRCTUsability(cf.at("can_use_rct").get<bool>());
        if (cf.contains("rct_proficiency")) s->SetRCTProficiency(cf.at("rct_proficiency").get<std::string>());
        character = std::move(s);
    }
    else if (type == "Cursed Spirit") {
        auto cs = std::make_unique<CursedSpirit>(
            st.at("hp").get<double>(),
            st.at("ce").get<double>(),
            st.at("regen").get<double>());
        if (cf.contains("passive_health_regen")) cs->SetPassiveRegen(cf.at("passive_health_regen").get<double>());
        character = std::move(cs);
    }
    else if (type == "Physically Gifted") {
        auto pg = std::make_unique<PhysicallyGifted>(
            st.at("hp").get<double>(),
            st.at("strength").get<double>());
        character = std::move(pg);
    }

    if (!character) return nullptr;
    
    if (id.contains("name")){
        character->SetCharacterName(id.at("name").get<std::string>());
    }
    if (id.contains("color")){
        character->SetCharacterColor(id.at("color").get<std::string>());
    }

    if (cf.contains("ai_type")) {
        character->SetBrain(GetBrainType(cf.at("ai_type").get<std::string>()));
    }
    if (cf.contains("attack_damage")) {
        character->SetBaseDamage(cf.at("attack_damage").get<double>());
    }

    if (character->IsaCurseUser()) { auto* curse_ptr = static_cast<CurseUser*>(character.get());
        // strings
        if (kt.contains("technique")) {
            curse_ptr->SetTechnique(GetTechniqueByName(kt.at("technique").get<std::string>()));
        }
        if (kt.contains("domain")) {
            curse_ptr->SetDomain(GetDomainByName(kt.at("domain").get<std::string>(), bc));
        }
        if (kt.contains("counter_domain")) {
            curse_ptr->SetCounterDomain(GetCounterDomainByName(kt.at("counter_domain").get<std::string>(), bc));
        }
        if (kt.contains("special")) {
            curse_ptr->SetSpecial(GetSpecialByName(kt.at("special").get<std::string>()));
        }
        if (kt.contains("shikigami") && kt.at("shikigami").is_array()) {
            for (const auto& name : kt.at("shikigami")) {
                curse_ptr->AddShikigami(GetShikigamiByName(name.get<std::string>()));
            }
        }
        // numerics
        if (tn.contains("blackflash_chance")) {
            curse_ptr->SetBlackflashChance(tn.at("blackflash_chance").get<int>());
        }
        if (tn.contains("domain_limit")) {
            curse_ptr->SetDomainLimit(tn.at("domain_limit").get<int>());
        }
        if (tn.contains("max_zone_time")) {
            curse_ptr->SetMaxZoneTime(tn.at("max_zone_time").get<int>());
        }
        if (tn.contains("max_reinforcement")) {
            curse_ptr->SetMaxReinforcement(tn.at("max_reinforcement").get<double>());
        }
        if (tn.contains("blackflash_multiplier")) {
            curse_ptr->SetBlackFlashMult(tn.at("blackflash_multiplier").get<double>());
        }
        if (tn.contains("max_burnout_time")) {
            curse_ptr->SetMaxBurnoutTime(tn.at("max_burnout_time").get<int>());
        }
    }

    if (tl.contains("equipped_tool")) {
        character->SetEquippedTool(GetToolByName(tl.at("equipped_tool").get<std::string>(), bc));
    }
    if (tl.contains("inventory") && tl.at("inventory").is_array()) {
        for (const auto& item : tl.at("inventory")) {
            character->AddToolToInventory(GetToolByName(item.get<std::string>(), bc));
        }
    }
    
    character->AssignID();
    return character;
}

void CharacterCreator::LoadJsonObject(BattleCreator& bc) {
    std::println(stderr, "Looking for characters.json in: {}", std::filesystem::current_path().string());
    std::ifstream file("characters.json");

    if (!file.is_open()) {
        std::println(stderr, "Could not find characters.json!");
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

    if (data.contains("characters") && data["characters"].is_array()) {
        for (const auto& charData : data["characters"]) {
            std::unique_ptr<Character> newChar = CharacterCreator::CreateJsonObject(charData, bc);
            if (newChar) {
                bc.characterlist.push_back(std::move(newChar));
            }
        }
        UserInterface::ClearScreen();
    }
}