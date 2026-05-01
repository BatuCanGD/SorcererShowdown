<p align="center">
  <img src="https://img.shields.io/badge/LINES%20OF%20CODE-5529-blue?style=for-the-badge" />
  <img src="https://img.shields.io/badge/FILES-97-yellow?style=for-the-badge" />
</p>

# ⚔️ Sorcerer Showdown

A Jujutsu Kaisen-inspired turn-based battle simulator written in **C++** using modules (`import std`). Fight as iconic sorcerers — or build your own custom characters, cursed techniques, and domains.

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

- **Requires** a C++23 compiler with `import std` support (MSVC `/std:c++latest` or Clang 17+)
- Easiest to use Visual Studio and add all `.cpp` files to one project

---

## 🕹 How to Play

1. Select your character and opponent count
2. Enable **Spectator Mode** (optional) for AI vs AI
3. Choose step-through or skip-turn mode for AI turns
4. On your turn, pick from 11 actions: Technique, Attack, Special, Domain, Taunt, RCT, DA, Tools, Technique Settings, Shikigami, Reinforcement

---

## 🧩 Adding Custom Content

There are two ways to expand the roster: writing a **native C++ class** for full control over AI behaviour and unique mechanics, or dropping a **`characters.json`** file in the same directory as the executable for quick, data-driven characters.

---

## 1. Native C++ Characters

### ➕ New Character

Pick your base class:

| Base Class | Use For |
|---|---|
| `Sorcerer` | CE user with RCT and a technique |
| `CursedSpirit` | CE entity without RCT |
| `PhysicallyGifted` | Heavenly-restricted, strength-based |

**MyCharacter.h:**
```cpp
#pragma once
#include "Sorcerer.h"

class MyCharacter : public Sorcerer {
public:
    MyCharacter();
    std::unique_ptr<Character> Clone() const override;
    void OnCharacterTurn(Character*, Battlefield&) override;
    bool CanBeHit() const override;
};
```

**MyCharacter.cpp:**
```cpp
#include "MyCharacter.h"
#include "BattlefieldHeader.h"
import std;

MyCharacter::MyCharacter() : Sorcerer(700.0, 3000.0, 100.0) {
    // hp, cursed_energy, ce_regen
    technique  = std::make_unique<MyTechnique>();
    domain     = std::make_unique<MyDomain>();
    rct_skill  = RCTProficiency::Adept;
    black_flash_chance = 10;
    base_attack_damage = 30.0;
    char_name = "My character";
    name_color = "\033[36m";
}
std::unique_ptr<Character> MyCharacter::Clone() const { return std::make_unique<MyCharacter>(); }
bool MyCharacter::CanBeHit() const              { return true; }

void MyCharacter::OnCharacterTurn(Character*, Battlefield& bf) {
    Character* target = nullptr;
    for (const auto& s : bf.battlefield) {
        if (s.get() != this) { target = s.get(); break; }
    }
    if (target) this->Attack(target);
}
```

Register in `BattleManager.cpp → SetupBattlefield` and include in `CharacterList.h`.

---

### ➕ New Cursed Technique

**MyTechnique.h:**
```cpp
#pragma once
#include "Techniques.h"

class MyTechnique : public Technique {
protected:
    static constexpr double output_damage = 80.0;
public:
    MyTechnique();
    void TechniqueMenu(CurseUser* user, Character* target, Battlefield&) override;
    std::unique_ptr<Technique> Clone() const override;
    void UseMyAbility(CurseUser* user, Character* target);
};
```

**MyTechnique.cpp:**
```cpp
#include "MyTechnique.h"
#include "CurseUser.h"
#include "Utils.h"
import std;

MyTechnique::MyTechnique(){
    tech_name = "My Technique";
    tech_color = "\033[32m";
}
void MyTechnique::UseMyAbility(CurseUser* user, Character* target) {
    // CalculateDamage deducts CE cost and applies Boost/Burnout multipliers automatically
    double dmg = CalculateDamage(user, output_damage);
    target->Damage(dmg);
    std::println("{} uses My Ability on {}!", user->GetNameWithID(), target->GetNameWithID());
}

void MyTechnique::TechniqueMenu(CurseUser* user, Character* target, Battlefield& bf) {
    if (user->DomainAmplificationActive()) { std::println("Blocked by DA!"); return; }
    std::println("1 - Use My Ability");
    std::print("=> ");
    if (GetValidInput() == 1) UseMyAbility(user, target);
}

std::unique_ptr<Technique> MyTechnique::Clone() const { return std::make_unique<MyTechnique>(*this); }
```

---

### ➕ New Domain

**MyDomain.h:**
```cpp
#pragma once
#include "Domain.h"

class MyDomain : public Domain {
public:
    MyDomain();
    void OnSureHit(CurseUser& user, Character& target) override;
};
```

**MyDomain.cpp:**
```cpp
#include "MyDomain.h"
#include "Character.h"
import std;

//           health   overwhelm_strength   range
MyDomain::MyDomain() : Domain(600.0, 100.0, 14.0) {
    ref_level = Refinement::Refined;      // Unstable / Crude / Refined / Absolute
    hit_type  = HitType::HitsCurseUsers;  // or HitsEveryone
    domain_name = "My Domain";
    domain_color = "\033[31m";
	domain_cost    = 400.0;
	surehit_damage = 90.0;
}

void MyDomain::OnSureHit(CurseUser& user, Character& target) {
    if (CheckDomainSurehit(target)) return; // handles counters, Heavenly Restriction, clashing
    target.DamageBypass(surehit_damage * DomainRangeMult());
    std::println("{} is struck inside {}!", target.GetNameWithID(), GetDomainName());
}
```

**Domain constructor params:** `health` (clash barrier HP), `overwhelm_strength` (damage to opposing domain per tick), `range` (larger range wins clash at equal refinement).

---

### ➕ New Cursed Tool

**MyTool.h:**
```cpp
#pragma once
#include "CursedTool.h"

class MyTool : public CursedTool{
public:
    MyTool();
    void UseTool(Character*, Character*) override;
    std::unique_ptr<CursedTool> Clone() const override;
}
```

**MyTool.cpp:**
```cpp
MyTool::MyTool(){
    tool_name = "My Tool";
    tool_color = "\033[35m";
}

void MyTool::UseTool(Character* user, Character* target) {
    // GetCalculatedStrength scales with Strength (PhysicallyGifted) or max HP (sorcerers)
    target->Damage(GetCalculatedStrength(user));
    std::println("{} attacks {} with {}!", user->GetNameWithID(), target->GetNameWithID(), GetName());
}

std::unique_ptr<CursedTool> MyTool::Clone() const {
	return std::make_unique<MyTool>(*this);
}
```

Add to `CursedToolList.h` and equip via `inventory_curse.push_back(std::make_unique<MyTool>())` in a character's constructor.

---

## 2. JSON Modding

Drop a file named `characters.json` next to the executable (or the built project directory) and the game will offer to load it at startup. JSON characters are built on the same base `Character` logic as native ones — they support all existing techniques, domains, tools, and shikigami — but their AI uses the default `OnCharacterTurn` fallback, so they won't tactically chain abilities or manage domains the way hand-coded characters do. Support for richer AI behaviour is planned for a future update.

> **Current limitations:** JSON cannot define new techniques, domains, or tools — only assign existing ones by name.

### Supported field reference

| Field | Type | Description |
|---|---|---|
| `name` | string | Display name |
| `type` | string | `"Sorcerer"`, `"Cursed Spirit"`, or `"Physically Gifted"` |
| `ai_type` | string | `"Aggressive"`, `"Reactive"` or `"Randomized"`; Necessary for the custom character to have a brain |
| `base_attack_damage` | float | Amount of damage that can be dealt by a character without techniques or cursed tools |
| `blackflash_chance` | int | The chance of a blackflash happening when attacking without cursed tools, domain amplification or cursed techniques |
| `hp` | float | Max health |
| `ce` | float | Max cursed energy (`"Physically Gifted"` ignores this) |
| `regen` | float | CE regen per turn (`"Physically Gifted"` ignores this) |
| `strength` | int | Strength stat — **required** for `"Physically Gifted"`, ignored otherwise |
| `six_eyes` | bool | Enables Six Eyes CE efficiency reduction (Sorcerer only) |
| `rct_proficiency` | string | `"None"`, `"Crude"`, `"Adept"`, `"Expert"`, or `"Absolute"` |
| `technique` | string | One of the available techniques listed below |
| `domain` | string | Main domain expansion |
| `counter_domain` | string | Counter-measure domain (Simple Domain / Hollow Wicker Basket) |
| `special` | string | Special move |
| `inventory` | string array | Cursed tools added to the character's inventory |
| `shikigami` | string array | Shikigami assigned to the character |
| `color` | string | ANSI escape code for the character's name colour |

### Available assets

| Category | Options |
|---|---|
| **Techniques** | `Limitless`, `Shrine`, `Private Pure Love Train`, `Idle Transfiguration`, `Copy` |
| **Domains** | `Infinite Void`, `Malevolent Shrine`, `Authentic Mutual Love`, `Idle Death Gamble` |
| **Counter Domains** | `Simple Domain`, `Hollow Wicker Basket` |
| **Specials** | `Unlimited Purple`, `World Cutting Slash` |
| **Tools** | `Inverted Spear of Heaven`, `Playful Cloud`, `Katana` |
| **Shikigami** | `Rika`, `Mahoraga`, `Agito` |

### Example `characters.json`

```json
{
  "characters": [
    {
      "name": "Legendary Six Eyes Wielder",
      "type": "Sorcerer",
      "ai_type": "Randomized",
      "base_attack_damage": 100.0,
      "blackflash_chance": 50,
      "hp": 3500.0,
      "ce": 20000.0,
      "regen": 300.0,
      "six_eyes": true,
      "rct_proficiency": "Absolute",
      "technique": "Limitless",
      "domain": "Infinite Void",
      "counter_domain": "Simple Domain",
      "special": "Unlimited Purple",
      "inventory": [],
      "shikigami": [
        "Rika",
        "Agito"
      ],
      "color": "\u001b[36m"
    },
    {
      "name": "Yuji Itadori",
      "type": "Sorcerer",
      "ai_type": "Reactive",
      "base_attack_damage": 300.0,
      "blackflash_chance": 100,
      "hp": 2000.0,
      "ce": 4000.0,
      "regen": 50.0,
      "six_eyes": false,
      "rct_proficiency": "Absolute",
      "technique": "Shrine",
      "domain": "Malevolent Shrine",
      "domain_limit": 10,
      "counter_domain": "Simple Domain",
      "inventory": [],
      "shikigami": [],
      "color": "\u001b[38;5;201m"
    },
    {
      "name": "Maki Zenin",
      "type": "Physically Gifted",
      "ai_type": "Aggressive",
      "base_attack_damage": 75.0,
      "hp": 5000.0,
      "strength": 1250.0,
      "inventory": [
        "The Inverted Spear of Heaven",
        "Playful Cloud",
        "Katana"
      ],
      "color": "\u001b[32m"
    }
  ]
}
```

---

## 🗂 Project Structure

```
SorcererShowdown/
├── Core
│   ├── Character           — Base class, HP/CE/reinforcement/tools
│   ├── CurseUser           — CE users, domain/technique/shikigami management
│   ├── Sorcerer            — Adds RCT proficiency, Six Eyes
│   ├── CursedSpirit        — Passive HP regen, no RCT
│   ├── PhysicallyGifted    — Strength-based combat, heavenly restriction
│   └── Shikigami           — Shadow/Partial/Full state machine
├── Systems
│   ├── Techniques          — Base class + CalculateDamage, chant, status
│   ├── Domain              — Base class + clash logic, surehit checks
│   ├── CursedTool          — Base tool + GetCalculatedStrength
│   ├── Specials            — One-off special move base
│   ├── BattleManager       — Game loop, domain resolution, turn management
│   ├── PlayerManager       — Player input routing and action handling
│   └── UIDisplay           — Status panels and action menus
├── Characters              — Gojo, Sukuna, Yuta, Hakari, Mahito, Toji, TransfiguredHuman
├── Techniques              — Limitless, Shrine, Copy, IdleTransfiguration, PrivatePureLoveTrain
├── Domains                 — InfiniteVoid, MalevolentShrine, AuthenticMutualLove,
│                             IdleDeathGamble, SelfEmbodimentOfPerfection,
│                             SimpleDomain, HollowWickerBasket
├── Shikigami               — Mahoraga, Rika, Agito
├── Tools                   — Katana, PlayfulCloud, InvertedSpearOfHeaven
└── SorcererShowdown.cpp    — main()
```

---

## ⚙️ Key Systems

**Domain Clashing** — Two active domains clash each turn. Higher `Refinement` wins outright; ties go to `Range`. Three+ domains all collapse.

**Burnout** — Deactivating a domain burns out the technique (35% output) for several turns.

**Black Flash** — Configurable per-character chance to deal 4x damage and boost technique to Domain Boost status.

**The Zone** — Sustaining Domain Boost outside a domain gives a temporary CE regen bonus.

**RCT Proficiency** — Tiers from None → Absolute determine heal amount and CE cost per RCT use.

**CE Reinforcement** — Reduces incoming damage at the cost of continuous CE drain each turn.

---

## 📝 License

Fan project based on Jujutsu Kaisen by Gege Akutami. All character names and concepts belong to their respective owners.
