<p align="center">
  <img src="https://img.shields.io/badge/LINES%20OF%20CODE-7007-blue?style=for-the-badge" />
  <img src="https://img.shields.io/badge/FILES-147-yellow?style=for-the-badge" />
</p>

# ⚔️ Sorcerer Showdown

A Jujutsu Kaisen-inspired turn-based battle simulator written in **C++23**. Fight as iconic sorcerers or build your own custom characters, cursed techniques, and domains.

---

## 🎮 Roster

| Character | Type | Cursed Technique | Domain |
|---|---|---|---|
| **Gojo** | Sorcerer | Limitless (Blue/Red/Purple) | Infinite Void |
| **Sukuna** | Sorcerer | Shrine (Dismantle/Cleave/WCS) | Malevolent Shrine |
| **Yuta** | Sorcerer | Copy | Authentic Mutual Love |
| **Hakari** | Sorcerer | Private Pure Love Train | Idle Death Gamble |
| **Mahito** | Cursed Spirit | Idle Transfiguration | Self Embodiment of Perfection |
| **Toji** | Physically Gifted | *(Heavenly Restricted)* | *(Heavenly Restricted)* |

---

## 🛠 Building

### Requirements

- C++23 compiler:
  - **MSVC** Visual Studio 2022 v17.6+ with `/std:c++latest`
  - **Clang** 18+ with libstdc++14 or libc++
  - **GCC** 14+
- CMake 3.28+
- Internet access on first build (CMake auto-downloads `json.hpp` from the nlohmann/json repo)

### CMake (recommended)

```bash
cmake -B build
cmake --build build
```

To explicitly choose your compiler:
```bash
cmake -B build -DCMAKE_CXX_COMPILER=g++-14
cmake -B build -DCMAKE_CXX_COMPILER=clang++
```

The executable lands in `build/`. If a `characters.json` exists in the project root, CMake copies it to the build directory automatically.

### Visual Studio (manual)

1. Create a new empty C++ project
2. Add all `.cpp` files from `code/source/` to the project
3. Add all subdirectories under `code/header/` to **Additional Include Directories**
4. Set **C++ Language Standard** to **ISO C++23** (or `/std:c++latest`)
5. Download [`json.hpp`](https://github.com/nlohmann/json/releases/download/v3.11.3/json.hpp) and place it in the project root

### Project layout expected by CMake

CMake sets the project root as the include base, so all `#include` paths in source files are relative to it (e.g. `"code/header/Characters/Character.h"`). New `.cpp` files placed anywhere under `code/source/` are picked up automatically.

```
SorcererShowdown/
├── CMakeLists.txt
├── json.hpp                  <- auto-downloaded if missing
├── characters.json           <- optional, copied to build directory
├── cursedtools.json          <- optional, copied to build directory
├── domains.json              <- optional, copied to build directory
└── code/
    ├── header/               <- all #includes are relative to the project root, not this folder
    └── source/               <- all .cpp files here are compiled automatically
```

---

## 🕹 How to Play

1. Select your character and opponent count
2. Enable **Spectator Mode** (optional) for AI vs AI
3. Choose step-through or skip-turn mode for AI turns
4. On your turn, pick from 12 actions: Technique, Attack, Special, Domain, Taunt, RCT, DA, Tools, Technique Settings, Shikigami, Reinforcement, Binding Vows

---

## 🧩 Adding Custom Content

Two paths: write a **native C++ class** for full control over AI behaviour and unique mechanics, or drop a **`characters.json`** file next to the executable for quick data-driven characters.

---

## 1. Native C++ Characters

### ➕ New Character

Pick your base class:

| Base Class | Use For |
|---|---|
| `Sorcerer` | CE user with RCT and a technique |
| `CursedSpirit` | CE entity without RCT, passive HP regen |
| `PhysicallyGifted` | Heavenly-restricted, strength-based combat |

**MyCharacter.h:**
```cpp
#pragma once
#include "code/header/Characters/CurseUsers/Sorcerers/Sorcerer.h"

class MyCharacter : public Sorcerer {
public:
    MyCharacter();
    std::unique_ptr<Character> Clone() const override;
    // override this for custom AI or omit it to use one of the generic brains instead
    void OnCharacterTurn(Battlefield& bf) override;
};
```

**MyCharacter.cpp:**
```cpp
#include "code/header/Characters/CurseUsers/Sorcerers/MyCharacter.h"
#include "code/header/GameManagement/BattlefieldHeader.h"
// include any technique/domain headers you use

MyCharacter::MyCharacter() : Sorcerer(700.0, 3000.0, 100.0) { // can be Sorcerer/PhysicallyGifted/CursedSpirit
    // Sorcerer inherits from CurseUser(hp, cursed_energy, ce_regen)
    technique          = std::make_unique<MyTechnique>();
    domain             = std::make_unique<MyDomain>();
    black_flash_chance = 10;
    attack_damage = 30.0;
    name          = "My Character";
    color         = "\033[36m";

    // optional RCT setup (Sorcerer only)
    rct_skill = RCTProficiency::Expert; // Crude / Adept / Expert / Absolute
    // optional passive regen (Cursed Spirit only)
    passive_health_regen = 40.0;
}

std::unique_ptr<Character> MyCharacter::Clone() const {
    return std::make_unique<MyCharacter>();
}

void MyCharacter::OnCharacterTurn(Battlefield& bf) {
    // full control over AI behaviour, write whatever logic you want here.
    // standard pattern: RCT -> technique -> attack
    if (!this->HPMoreThanMax(0.50) && this->CEMoreThanMax(0.20)) {
        this->BoostRCT();
    } else {
        this->DisableRCT();
    }
    this->UseRCT();

    Character* target = nullptr;
    for (const auto& t : bf.battlefield){
        if (t.get() == this) continue;
        target = t.get(); // pick a target but not ourselves
    }

    if (this->GetTechnique() && !this->GetTechnique()->BurntOut()) {
        if (this->GetTechnique()->AutoTechniqueUse(this, target, bf)) return;
    }
    this->Attack(target);
}
```

**Don't want custom AI?** Skip `OnCharacterTurn` entirely and assign one of the four generic brains in the constructor instead, the base `Character::OnCharacterTurn` will dispatch to it automatically:
```cpp
#include "code/header/CharacterCreator/AI/Aggressive.h" // or Reactive / Randomized / Brawler

MyCharacter::MyCharacter() : Sorcerer(700.0, 3000.0, 100.0) {
    this->SetBrain(std::make_unique<Aggressive>());
}
```

**Register:** add your character to the roster by including its header in `CharacterList.h` and pushing it into `bc.characterlist` inside `BattleManager::loadSetup` in `BattleManager.cpp`:
```cpp
bc.characterlist.push_back(std::make_unique<MyCharacter>());
```

---

### ➕ New Cursed Technique

`Technique` has two **pure virtual** methods you must implement. `TechniqueMenu` (player input path) and `AutoTechniqueUse` (AI path), plus `Clone`. `Chant` and `TechniqueSetting` have default no-op implementations and are optional.

**MyTechnique.h:**
```cpp
#pragma once
#include "code/header/Techniques/Techniques.h"

class MyTechnique : public Technique {
protected:
    static constexpr double output_damage = 80.0;
public:
    MyTechnique();
    std::unique_ptr<Technique> Clone() const override;

    void UseMyAbility(CurseUser* user, Character* target);

    // required overrides
    void TechniqueMenu(CurseUser* user, Character* target, Battlefield&) override;
    bool AutoTechniqueUse(CurseUser* user, Character* target, Battlefield&) override;

    // optional overrides
    void Chant() override;
    void TechniqueSetting(CurseUser*, Battlefield&) override;
};
```

**MyTechnique.cpp:**
```cpp
#include "code/header/Techniques/MyTechnique.h"
#include "code/header/Characters/CurseUsers/CurseUser.h"
#include "code/header/GameManagement/Utils.h"

MyTechnique::MyTechnique() {
    name  = "My Technique";
    color = "\033[32m";
}

std::unique_ptr<Technique> MyTechnique::Clone() const {
    return std::make_unique<MyTechnique>(*this);
}

void MyTechnique::UseMyAbility(CurseUser* user, Character* target) {
    // CalculateDamage(user, cost) deducts CE, applies Boost/BurntOut multipliers,
    // warns if CE is insufficient, and returns the final damage value
    double dmg = CalculateDamage(user, output_damage);
    target->Damage(dmg);
    std::println("{} uses My Ability on {}!", user->GetNameWithID(), target->GetNameWithID());
}

// Player input path
void MyTechnique::TechniqueMenu(CurseUser* user, Character* target, Battlefield& bf) {
    if (user->DomainAmplificationActive()) {
        std::println("Blocked by Domain Amplification!");
        return;
    }
    std::println("1 - Use My Ability");
    std::print("=> ");
    if (Utilities::GetInput<int>() == 1) UseMyAbility(user, target);
}

// AI path - called automatically each turn
bool MyTechnique::AutoTechniqueUse(CurseUser* user, Character* target, Battlefield& bf) {
    UseMyAbility(user, target);
    return true;
}

// Optional: advance chant level each call (Zero → One → Two → Three → Four)
// GetChantPower() returns 1.0 + (chant_level * 0.50) use as a damage multiplier
void MyTechnique::Chant() {
    if (chant == ChantLevel::Zero) {
        std::println("\"First verse...\"");
        chant = ChantLevel::One;
    } else if (chant == ChantLevel::Four) {
        std::println("Technique is at maximum output!");
    }
    // add further stages as needed
}

// Optional: shown via action 9 (Technique Settings) in-game
void MyTechnique::TechniqueSetting(CurseUser* user, Battlefield& bf) {
    std::println("No extra settings.");
}
```

**Technique status** is tracked via `Technique::Status`:

| Status | `CalculateDamage` multiplier | Set when |
|---|---|---|
| `Usable` | 1× | Default; restored after burnout recovery |
| `DomainBoost` | 2× | Black Flash lands, or domain activates |
| `BurntOut` | 0.35× | Domain deactivates |

Check with `Usable()`, `Boosted()`, `BurntOut()`. The base `Set(Status)` propagates status, override it if you need to forward it to sub-techniques (see `Copy::Set` for an example).

Register by adding to `GetTechniqueByName` in `CharacterCreator.cpp` for JSON support.

---

### ➕ New Domain

**MyDomain.h:**
```cpp
#pragma once
#include "code/header/Domains/Domain.h"

class MyDomain : public Domain {
public:
    MyDomain();
    std::unique_ptr<Domain> Clone() const override;
    void OnSureHit(CurseUser& user, Character& target) override;
};
```

**MyDomain.cpp:**
```cpp
#include "code/header/Domains/MyDomain.h"
#include "code/header/Characters/Character.h"

//              health   overwhelm_strength   range
MyDomain::MyDomain() : Domain(600.0, 100.0, 14) {
    ref_level    = Refinement::Refined;    // Unstable / Crude / Refined / Absolute
    hit_type     = HitType::HitCurseUser; // HitCurseUser / HitAll / HitAllSoul
    name         = "My Domain";
    color        = "\033[31m";
    domain_cost    = 400.0;  // CE drained from the user each turn while active
    surehit_damage = 90.0;   // base damage applied in OnSureHit
}

std::unique_ptr<Domain> MyDomain::Clone() const {
    return std::make_unique<MyDomain>(*this);
}

void MyDomain::OnSureHit(CurseUser& user, Character& target) {
    // IsSurehitBlocked handles: counter-domain protection and/or Heavenly Restricted users based on the type of domain surehit
    if (IsSurehitBlocked(target)) return;
    // DamageBypass skips over techniques that block damage like infinity
    target.DamageBypass(surehit_damage);
    std::println("{} is struck inside {}!", target.GetNameWithID(), GetDomainName());
}
```

**Constructor parameters:**

| Param | Effect |
|---|---|
| `health` | Barrier HP consumed during clashes |
| `overwhelm_strength` | Damage dealt to the opposing domain's barrier per clash tick |
| `range` | At equal `Refinement`, higher range wins the clash |

**Clash resolution order:**
1. `Refinement` mismatch -> higher refinement wins outright; lower domain collapses immediately
2. Equal refinement -> `range` comparison; losing domain takes `overwhelm_strength` damage per tick
3. Equal range -> both domains take `0.5 × overwhelm_strength` per tick (stalemate)
4. Three or more active domains → all collapse simultaneously

**`HitType` options:**

| Value | Who `OnSureHit` targets |
|---|---|
| `HitCurseUser` | CE users only; `PhysicallyGifted` targets are immune (Heavenly Restriction) |
| `HitAll` | All characters including `PhysicallyGifted` |
| `HitAllSoul` | All characters, damage bypasses both CE reinforcement and Infinity (`DamageBypassAll`) |

Register by adding to `GetDomainByName` or `GetCounterDomainByName` in `CharacterCreator.cpp` for JSON support.

---

### ➕ New Cursed Tool

**MyTool.h:**
```cpp
#pragma once
#include "code/header/CursedTools/CursedTool.h"

class MyTool : public CursedTool {
public:
    MyTool();
    std::unique_ptr<CursedTool> Clone() const override;
    double GetCalculatedStrength(Character*) const override; // optional, only if you want custom scaling
};
```

**MyTool.cpp:**
```cpp
#include "code/header/CursedTools/MyTool.h"
#include "code/header/Characters/PhysicallyGifted/PhysicallyGifted.h"
#include "code/header/GameManagement/Utils.h"

MyTool::MyTool() {
    name       = "My Tool";
    color      = "\033[35m";
    base_damage = 60.0;
    type       = WeaponType::Normal; // Normal / TechniqueBypass / ReinforcementBypass / AllBypass
}

// Optional: Only override this if the default CE/strength scaling doesn't fit.
// Default: PhysicallyGifted gets base_damage + strength * 2.5, others get base_damage + sqrt(CE) * ~1.0
double MyTool::GetCalculatedStrength(Character* user) const {
    if (user->IsPhysicallyGifted()) {
        return base_damage + (static_cast<PhysicallyGifted*>(user)->GetStrengthDamage() * 3.0);
    }
    return base_damage * 1.5; // flat multiplier for CE users
}

std::unique_ptr<CursedTool> MyTool::Clone() const {
    return std::make_unique<MyTool>(*this);
}
```

`UseTool` is handled by the base class and doesn't need to be overridden. It automatically applies the correct damage function based on the `WeaponType`, checks for Infinity, and prints the attack message. The `WeaponType` controls behaviour:

| Type | Effect |
|---|---|
| `Normal` | Damage reduced by target's CE reinforcement |
| `TechniqueBypass` | Pierces Infinity; damage still reduced by reinforcement |
| `ReinforcementBypass` | Hits the soul directly, bypasses reinforcement |
| `AllBypass` | Pierces both Infinity and reinforcement |

Register by adding to `GetToolByName` in `CharacterCreator.cpp` for JSON support. Equip via:
```cpp
inventory_curse.push_back(std::make_unique<MyTool>()); // in inventory, unequipped
cursed_tool = std::make_unique<MyTool>();               // equipped at battle start
```

---

## 2. JSON Modding

Drop files named `characters.json`, `cursedtools.json`, and/or `domains.json` next to the executable and the game will load them at startup. JSON characters support all existing and custom-defined techniques, domains, tools, and shikigami, but since they use one of the four generic AI brains they won't tactically chain abilities the way hand-coded characters do.

> **Current limitations:** JSON cannot define new techniques or shikigami. It can only assign existing ones by name. Custom domains and cursed tools defined in `domains.json` and `cursedtools.json` are fully usable by name in `characters.json`.

### Supported field reference

**identity**

| Field | Type | Description |
|---|---|---|
| `name` | string | Display name |
| `color` | string | ANSI escape code for name colour |

**stats**

| Field | Type | Description |
|---|---|---|
| `hp` | float | Max health |
| `ce` | float | Max cursed energy. `Curse Users` only |
| `regen` | float | CE regen per turn. `Curse Users` only |
| `strength` | float | Strength stat. **Required** for `"Physically Gifted"`, ignored otherwise |

**config**

| Field | Type | Description |
|---|---|---|
| `type` | string | `"Sorcerer"`, `"Cursed Spirit"`, or `"Physically Gifted"`. Read from the root object, not inside `"config"` |
| `ai_type` | string | `"Aggressive"`, `"Reactive"`, `"Randomized"`, or `"Brawler"`. **Required** for the character to act |
| `attack_damage` | float | Damage dealt by unarmed attacks without techniques or tools |
| `six_eyes` | bool | Six Eyes CE efficiency. Reduces CE costs to ~30% (Sorcerer only) |
| `can_use_rct` | bool | A boolean to enable a Sorcerer to use Reverse Cursed Technique |
| `rct_proficiency` | string | `"Crude"`, `"Adept"`, `"Expert"`, or `"Absolute"`. Unrecognized strings default to `Adept` |
| `passive_health_regen` | float | HP regained per turn. `"Cursed Spirit"` only |

**sorcery › tuning**

| Field | Type | Description |
|---|---|---|
| `blackflash_chance` | int | % chance of Black Flash on a standard attack |
| `max_domain_time` | int | The maximum amount of time a domain can stay active (can still get shattered and deactivated) |
| `max_zone_time` | int | The maximum amount of time a character will stay in the domain boosted state after hitting a blackflash |
| `max_reinforcement` | double | The maximum value of how much a character can reinforce themselves with CE, thus reducing oncoming damage |
| `blackflash_multiplier` | double | The value of how much the base attack damage will be multiplied by if the attack is a blackflash |
| `max_burnout_time` | int | The amount of turns the character's technique will be burnt out for |
| `domain_limit` | int | Max domain activations before overuse penalty kicks in (default 5) |

**sorcery › kit**

| Field | Type | Description |
|---|---|---|
| `technique` | string | Assigned cursed technique |
| `domain` | string | Main domain expansion |
| `counter_domain` | string | Counter-measure domain |
| `special` | string | Special move |
| `shikigami` | string array | Shikigami assigned to the character |

**tools**

| Field | Type | Description |
|---|---|---|
| `equipped_tool` | string | Tool equipped at battle start |
| `inventory` | string array | Tools in the character's inventory (unequipped) |

---

### Available assets

| Category | Options |
|---|---|
| **Techniques** | `Limitless`, `Shrine`, `Private Pure Love Train`, `Idle Transfiguration`, `Copy` |
| **Domains** | `Infinite Void`, `Malevolent Shrine`, `Authentic Mutual Love`, `Idle Death Gamble`, `Self Embodiment Of Perfection` |
| **Counter Domains** | `Simple Domain`, `Hollow Wicker Basket` |
| **Specials** | `Unlimited Purple`, `World Cutting Slash` |
| **Tools** | `The Inverted Spear of Heaven`, `Playful Cloud`, `Katana`, `Split Soul Katana` |
| **Shikigami** | `Rika`, `Mahoraga`, `Agito` |

Custom domains and cursed tools defined in `domains.json` and `cursedtools.json` are loaded first, so they can be referenced by name in `characters.json` exactly like the built-in ones.

### Example `characters.json`

```json
{
  "characters": [
    {
      "type": "Sorcerer",
      "identity": {
        "name": "Legendary Six Eyes Wielder",
        "color": "\u001b[36m"
      },
      "stats": {
        "hp": 3500.0,
        "ce": 20000.0,
        "regen": 300.0
      },
      "config": {
        "attack_damage": 100.0,
        "ai_type": "Randomized",
        "six_eyes": true,
        "can_use_rct": true,
        "rct_proficiency": "Absolute"
      },
      "sorcery": {
        "kit": {
          "technique": "Limitless",
          "domain": "Infinite Void",
          "counter_domain": "Simple Domain",
          "special": "Unlimited Purple",
          "shikigami": [
            "Rika",
            "Agito"
          ]
        },
        "tuning": {
          "blackflash_chance": 50
        }
      },
      "tools": {
        "equipped_tool": "",
        "inventory": []
      }
    },
    {
      "type": "Sorcerer",
      "identity": {
        "name": "Yuji Itadori",
        "color": "\u001b[38;5;201m"
      },
      "stats": {
        "hp": 2000.0,
        "ce": 4000.0,
        "regen": 2500.0
      },
      "config": {
        "attack_damage": 300.0,
        "ai_type": "Brawler",
        "can_use_rct": true,
        "rct_proficiency": "Absolute"
      },
      "sorcery": {
        "kit": {
          "technique": "Shrine",
          "domain": "Cutting Thorns",
          "counter_domain": "Simpler Domain",
          "shikigami": []
        },
        "tuning": {
          "blackflash_chance": 100,
          "blackflash_multiplier": 10.0,
          "domain_limit": 10,
          "max_burnout_time": 0,
          "max_domain_time": 999,
          "max_reinforcement": 1000.0,
          "max_zone_time": 999
        }
      },
      "tools": {
        "equipped_tool": "",
        "inventory": [
          "The Black Blade"
        ]
      }
    },
    {
      "type": "Physically Gifted",
      "identity": {
        "name": "Maki Zenin",
        "color": "\u001b[32m"
      },
      "stats": {
        "hp": 5000.0,
        "strength": 1250.0
      },
      "config": {
        "attack_damage": 75.0,
        "ai_type": "Aggressive"
      },
      "tools": {
        "equipped_tool": "",
        "inventory": [
          "The Inverted Spear of Heaven",
          "Playful Cloud",
          "Katana"
        ]
      }
    }
  ]
}
```

---

### `cursedtools.json`

Drop a `cursedtools.json` next to the executable to define custom cursed tools. These are loaded at startup and can then be referenced by name in `characters.json` via `"equipped_tool"` or `"inventory"`.

**identity**

| Field | Type | Description |
|---|---|---|
| `name` | string | Display name (must match exactly when referenced in characters.json) |
| `color` | string | ANSI escape code for name colour |

**stats**

| Field | Type | Description |
|---|---|---|
| `type` | string | `"Normal"`, `"Bypass Techniques"`, `"Bypass Reinforcement"`, or `"Bypass Everything"` |
| `damage` | float | Base damage before strength/CE scaling |

**Tool types explained**
- `Normal`: damage is reduced by the target's CE reinforcement
- `Bypass Techniques`: pierces Infinity; damage still reduced by reinforcement
- `Bypass Reinforcement`: hits the soul directly, bypasses reinforcement
- `Bypass Everything`: pierces both Infinity and reinforcement

```json
{
  "cursedtools": [
    {
      "identity": {
        "name": "Whimsical Cloud",
        "color": "\u001b[31m"
      },
      "stats": {
        "type": "Normal",
        "damage": 65.0
      }
    },
    {
      "identity": {
        "name": "Spliced Spear of Heaven",
        "color": "\u001b[35m"
      },
      "stats": {
        "type": "Bypass Techniques",
        "damage": 40.0
      }
    },
    {
      "identity": {
        "name": "Split Essence Katana",
        "color": "\u001b[36m"
      },
      "stats": {
        "type": "Bypass Reinforcement",
        "damage": 55.0
      }
    }
  ]
}
```

---

### `domains.json`

Drop a `domains.json` next to the executable to define custom domains and counter-domains. These are loaded at startup and can then be referenced by name in `characters.json` via `"domain"` or `"counter_domain"`.

**identity**

| Field | Type | Description |
|---|---|---|
| `name` | string | Display name. Must match exactly when referenced in `characters.json` |
| `color` | string | ANSI escape code for name colour |

**stats**

| Field | Type | Description |
|---|---|---|
| `health` | float | Domain barrier HP. Reduced during clashes |
| `strength` | float | Barrier overwhelming power dealt to the opponent's domain per clash tick |
| `range` | int | Domain range. Used as tiebreaker in clashes of equal refinement |

**config**

| Field | Type | Description |
|---|---|---|
| `is_neutralizer` | bool | Set `true` to make this a counter-domain (Simple Domain / Hollow Wicker Basket type). Neutralizers don't deal surehit damage. |
| `domain_type` | string | `"Hits Everyone"`, `"Hits Soul"`, or omit for the default (curse users only). Ignored for neutralizers |
| `refinement` | string | `"Unstable"`, `"Crude"`, `"Refined"`, or `"Absolute"`. Determines clash priority |
| `cost` | float | CE drained per turn while the domain is active |
| `can_stun` | bool | Whether the surehit stuns the target |
| `surehit_damage` | float | Damage dealt to each target caught inside the domain each turn |

```json
{
  "domains": [
    {
      "identity": {
        "name": "Expanding Void",
        "color": "\u001b[36m"
      },
      "stats": {
        "health": 250.0,
        "strength": 250.0,
        "range": 35
      },
      "config": {
        "domain_type": "Hits Everyone",
        "refinement": "Crude",
        "cost": 1000.0,
        "can_stun": true,
        "surehit_damage": 100.0
      }
    },
    {
      "identity": {
        "name": "Cutting Thorns",
        "color": "\u001b[32m"
      },
      "stats": {
        "health": 500.0,
        "strength": 300.0,
        "range": 25
      },
      "config": {
        "refinement": "Refined",
        "cost": 950.0,
        "can_stun": false,
        "surehit_damage": 90.0
      }
    },
    {
      "identity": {
        "name": "All Seeing Horus",
        "color": "\u001b[38;5;201m"
      },
      "stats": {
        "health": 4000.0,
        "strength": 300.0,
        "range": 30
      },
      "config": {
        "domain_type": "Hits Soul",
        "refinement": "Absolute",
        "cost": 900.0,
        "can_stun": false,
        "surehit_damage": 500.0
      }
    },
    {
      "identity": {
        "name": "Simpler Domain",
        "color": "\u001b[34m"
      },
      "config": {
        "is_neutralizer": true
      }
    }
  ]
}
```


---

## 🗂 Project Structure

```
SorcererShowdown/
├── Core
│   ├── Character           ├ Base class: HP, tools, stun, brain dispatch
│   ├── CurseUser           ├ CE, domain/technique/shikigami management, Binding Vows, Black Flash
│   ├── Sorcerer            ├ RCT proficiency tiers, Six Eyes CE efficiency
│   ├── CursedSpirit        ├ Passive HP regen per turn, no RCT
│   ├── PhysicallyGifted    ├ Strength-based damage/defence, Heavenly Restriction
│   └── Shikigami           ├ Shadow / Partial / Full state machine
├── Systems                 |
│   ├── Techniques          ├ Base class: CalculateDamage, chant levels, status
│   ├── Domain              ├ Base class: clash resolution, surehit dispatch
│   ├── CursedTool          ├ Base tool: GetCalculatedStrength scaling
│   ├── Specials            ├ One-off special move base
│   ├── CharacterAI         ├ CharacterBrain: Aggressive / Reactive / Randomized / Brawler
│   ├── BattleManager       ├ Game loop, domain resolution, turn management
│   ├── PlayerManager       ├ Player input routing and action handling
│   └── UserInterface       ├ Status panels and action menus
├── Characters              ├ Gojo, Sukuna, Yuta, Hakari, Mahito, Toji, TransfiguredHuman
├── Techniques              ├ Limitless, Shrine, Copy, IdleTransfiguration, PrivatePureLoveTrain
├── Domains                 ├ InfiniteVoid, MalevolentShrine, AuthenticMutualLove,
│                           | IdleDeathGamble, SelfEmbodimentOfPerfection,
│                           | SimpleDomain, HollowWickerBasket
├── Shikigami               ├ Mahoraga (World Cutting Slash Unlock (Shrine Technique only)), Rika (CE amplifier), Agito (passive heal)
├── Tools                   ├ Katana, PlayfulCloud, InvertedSpearOfHeaven, SplitSoulKatana
├── Binding Vows            ├ Brittle Efficiency, Cursed Energy Sacrifice, Bare-Handed
└── SorcererShowdown.cpp    ├ main()
```

---

## ⚙️ Key Systems

**Domain Clashing**: Two active domains clash each turn. Higher `Refinement` wins outright; equal refinement goes to `Range`. Equal range is a stalemate. Three or more active domains all collapse simultaneously.

**Binding Vows**: Using a Binding Vow exchanges something equal value to get something of equal value such as trading `Reinforcement` capacity to lessen the cost of using `Reinforcement`.

**Burnout**: Deactivating a domain burns out the technique (0.35× output) for several turns. `RecoverTechniqueBurnout` ticks each end-of-turn until the technique resets to `Usable`.

**Black Flash**: Configurable per-character chance on a standard attack. On hit, it clears burnout and `is_strained`, boosts technique status to `DomainBoost`, and increments a chain counter. Damage is `attack_damage × (blackflash_mult × chain)`, so consecutive Black Flashes scale up. Missing one resets the chain to zero.

**The Zone**: Sustaining `DomainBoost` status outside an active domain grants a temporary CE regen bonus for up to 3 turns before resetting to `Usable`.

**RCT Proficiency**: Tiers from `None` → `Absolute` determine heal amount and CE cost per RCT use. `Overdrive` mode doubles both heal and cost.

**CE Reinforcement**: Divides incoming damage by up to 3× at max reinforcement (`1.0 + (current / max) * 2`). Each turn, maintaining reinforcement costs `current_reinforcement × reinforcement_cost_mult` CE (default `2.0`, varies per character); if CE drops below the reinforcement value, the reinforcement collapses entirely.

---

## 📝 License

Fan project based on Jujutsu Kaisen by Gege Akutami. All character names and concepts belong to their respective owners.
