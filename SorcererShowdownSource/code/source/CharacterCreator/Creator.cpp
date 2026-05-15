#include "code/header/CharacterCreator/CharacterCreator.h"
#include "code/header/CharacterCreator/AI/CharacterAI.h"
#include "code/header/CharacterCreator/AI/Aggressive.h"
#include "code/header/CharacterCreator/AI/Reactive.h"
#include "code/header/CharacterCreator/AI/Brawler.h"
#include "code/header/CharacterCreator/AI/Randomized.h"

#include "code/header/GameManagement/BattlefieldHeader.h"

#include "code/header/Characters/Shikigami/Rika.h"
#include "code/header/Characters/CurseUsers/CurseUser.h"
#include "code/header/Characters/CurseUsers/Sorcerers/Sorcerer.h"
#include "code/header/Characters/CurseUsers/CursedSpirits/CursedSpirit.h"
#include "code/header/Characters/PhysicallyGifted/PhysicallyGifted.h"
#include "code/header/Characters/Shikigami/Mahoraga.h"
#include "code/header/Characters/Shikigami/Agito.h"

#include "code/header/Techniques/Limitless.h"
#include "code/header/Techniques/Shrine.h"
#include "code/header/Techniques/IdleTransfiguration.h"
#include "code/header/Techniques/PrivatePureLoveTrain.h"
#include "code/header/Techniques/Copy.h"

#include "code/header/Domains/MalevolentShrine.h"
#include "code/header/Domains/AuthenticMutualLove.h"
#include "code/header/Domains/IdleDeathGamble.h"
#include "code/header/Domains/SimpleDomain.h"
#include "code/header/Domains/HollowWickerBasket.h"
#include "code/header/Domains/SelfEmbodimentOfPerfection.h"
#include "code/header/Domains/InfiniteVoid.h"

#include "code/header/CursedTools/InvertedSpearOfHeaven.h"
#include "code/header/CursedTools/Katana.h"
#include "code/header/CursedTools/PlayfulCloud.h"

#include "code/header/Specials/UnlimitedPurple.h"
#include "code/header/Specials/WorldCuttingSlash.h"

#include "code/header/GameManagement/UserInterface.h"

static std::unique_ptr<Technique> GetTechniqueByName(const std::string& name);
static std::unique_ptr<CharacterBrain> GetBrainType(const std::string& name);
static std::unique_ptr<Domain> GetDomainByName(const std::string& name);
static std::unique_ptr<Domain> GetCounterDomainByName(const std::string& name);
static std::unique_ptr<Specials> GetSpecialByName(const std::string& name);
static std::unique_ptr<CursedTool> GetToolByName(const std::string& name);
static std::unique_ptr<Shikigami> GetShikigamiByName(const std::string& name);

std::unique_ptr<Character> CharacterCreator::CreateFromJson(const json& j) {
    std::string type = j.at("type").get<std::string>();
    std::unique_ptr<Character> character;

    if (type == "Sorcerer") {
        auto s = std::make_unique<Sorcerer>(
            j.at("hp").get<double>(),
            j.at("ce").get<double>(),
            j.at("regen").get<double>());

        if (j.contains("six_eyes")) s->SetSixEyes(j.at("six_eyes").get<bool>());
        if (j.contains("rct_proficiency")) s->SetRCTProficiency(j.at("rct_proficiency").get<std::string>());

        character = std::move(s);
    }
    else if (type == "Cursed Spirit") {
        auto cs = std::make_unique<CursedSpirit>(
            j.at("hp").get<double>(),
            j.at("ce").get<double>(),
            j.at("regen").get<double>());
        if (j.contains("passive_health_regen")) cs->SetPassiveRegen(j.at("passive_health_regen").get<double>());
        character = std::move(cs);
    }
    else if (type == "Physically Gifted") {
        auto pg = std::make_unique<PhysicallyGifted>(
            j.at("hp").get<double>(),
            j.at("strength").get<double>());
        character = std::move(pg);
    }

    if (!character) return nullptr;

    if (j.contains("ai_type")) {
        character->SetBrain(GetBrainType(j.at("ai_type").get<std::string>()));
    }

    if (j.contains("base_attack_damage")) {
        character->SetBaseDamage(j.at("base_attack_damage").get<double>());
    }

    if (character->IsaCurseUser()) {
        auto* curse_ptr = static_cast<CurseUser*>(character.get());
        if (j.contains("blackflash_chance")) {
            curse_ptr->SetBlackflashChance(j.at("blackflash_chance").get<int>());
        }
        if (j.contains("technique")) {
            curse_ptr->SetTechnique(GetTechniqueByName(j.at("technique").get<std::string>()));
        }
        if (j.contains("domain")) {
            curse_ptr->SetDomain(GetDomainByName(j.at("domain").get<std::string>()));
        }
        if (j.contains("domain_limit")) {
            curse_ptr->SetDomainLimit(j.at("domain_limit").get<int>());
        }
        if (j.contains("counter_domain")) {
            curse_ptr->SetCounterDomain(GetCounterDomainByName(j.at("counter_domain").get<std::string>()));
        }
        if (j.contains("special")) {
            curse_ptr->SetSpecial(GetSpecialByName(j.at("special").get<std::string>()));
        }
        if (j.contains("shikigami") && j.at("shikigami").is_array()) {
            for (const auto& name : j.at("shikigami")) {
                curse_ptr->AddShikigami(GetShikigamiByName(name.get<std::string>()));
            }
        }
    }
    if (j.contains("equipped_tool")) {
        character->SetEquippedTool(GetToolByName(j.at("equipped_tool").get<std::string>()));
    }
    if (j.contains("inventory") && j.at("inventory").is_array()) {
        for (const auto& item : j.at("inventory")) {
            character->AddToolToInventory(GetToolByName(item.get<std::string>()));
        }
    }
    character->SetCharacterName(j.at("name").get<std::string>(), j.value("color", ""));
    character->AssignID();
    return character;
}

void CharacterCreator::LoadJsonCharacter(BattleCreator& bc) {
    std::cout << "Looking for JSON in: " << std::filesystem::current_path() << '\n';
    std::ifstream file("characters.json");

    if (!file.is_open()) {
        std::cerr << "Could not find characters.json!" << '\n';
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
        return;
    }
    nlohmann::json data;
    try {
        file >> data;
    }
    catch (const nlohmann::json::parse_error& e) {
        std::cerr << "JSON Parse Error: " << e.what() << '\n';
        return;
    }

    if (data.contains("characters") && data["characters"].is_array()) {
        for (const auto& charData : data["characters"]) {
            std::unique_ptr<Character> newChar = CharacterCreator::CreateFromJson(charData);
            if (newChar) {
                bc.characterlist.push_back(std::move(newChar));
            }
        }
        UserInterface::ClearScreen();
    }
}

static std::unique_ptr<Technique> GetTechniqueByName(const std::string& name) {
    if (name == "Limitless") return std::make_unique<Limitless>();
    if (name == "Shrine") return std::make_unique<Shrine>();
    if (name == "Private Pure Love Train") return std::make_unique<PrivatePureLoveTrain>();
    if (name == "Idle Transfiguration") return std::make_unique<IdleTransfiguration>();
    if (name == "Copy") return std::make_unique<Copy>();
    return nullptr;
}

static std::unique_ptr<CharacterBrain> GetBrainType(const std::string& name) {
    if (name == "Aggressive") return std::make_unique<Aggressive>();
    if (name == "Reactive") return std::make_unique<Reactive>();
    if (name == "Randomized") return std::make_unique<Randomized>();
    if (name == "Brawler") return std::make_unique<Brawler>();
    return std::make_unique<Aggressive>();
}

static std::unique_ptr<Domain> GetDomainByName(const std::string& name) {
    if (name == "Infinite Void") return std::make_unique<InfiniteVoid>();
    if (name == "Malevolent Shrine") return std::make_unique<MalevolentShrine>();
    if (name == "Authentic Mutual Love") return std::make_unique<AuthenticMutualLove>();
    if (name == "Idle Death Gamble") return std::make_unique<IdleDeathGamble>();
    if (name == "Self Embodiment of Perfection") return std::make_unique<SelfEmbodimentOfPerfection>();
    return nullptr;
}
static std::unique_ptr<Domain> GetCounterDomainByName(const std::string& name) {
    if (name == "Simple Domain") return std::make_unique<SimpleDomain>();
    if (name == "Hollow Wicker Basket") return std::make_unique<HollowWickerBasket>();
    return nullptr;
}

static std::unique_ptr<Specials> GetSpecialByName(const std::string& name) {
    if (name == "Unlimited Purple") return std::make_unique<UnlimitedPurple>();
    if (name == "World Cutting Slash") return std::make_unique<WorldCuttingSlash>();
    return nullptr;
}

static std::unique_ptr<CursedTool> GetToolByName(const std::string& name) {
    if (name == "The Inverted Spear of Heaven") return std::make_unique<InvertedSpearofHeaven>();
    if (name == "Playful Cloud") return std::make_unique<PlayfulCloud>();
    if (name == "Katana") return std::make_unique<Katana>();
    return nullptr;
}

static std::unique_ptr<Shikigami> GetShikigamiByName(const std::string& name) {
    if (name == "Agito") return std::make_unique<Agito>();
    if (name == "Mahoraga") return std::make_unique<Mahoraga>();
    if (name == "Rika") return std::make_unique<Rika>();
    return nullptr;
}