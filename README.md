<p align="center">
  <img src="https://img.shields.io/badge/LINES%20OF%20CODE-6443-blue?style=for-the-badge" />
  <img src="https://img.shields.io/badge/FILES-128-yellow?style=for-the-badge" />
</p>

# Sorcerer Showdown

A Jujutsu Kaisen-inspired turn-based battle simulator written in **C++23**. Fight as iconic characters or build your own custom characters, cursed tools and domains.

---

## Roster

| Character | Type | Cursed Technique | Domain |
|---|---|---|---|
| **Gojo** | Sorcerer | Limitless (Blue/Red/Purple) | Infinite Void |
| **Sukuna** | Sorcerer | Shrine (Dismantle/Cleave/WCS) | Malevolent Shrine |
| **Yuta** | Sorcerer | Copy | Authentic Mutual Love |
| **Hakari** | Sorcerer | Private Pure Love Train | Idle Death Gamble |
| **Mahito** | Cursed Spirit | Idle Transfiguration | Self Embodiment of Perfection |
| **Toji** | Physically Gifted | *(Heavenly Restricted)* | *(Heavenly Restricted)* |

---

## Key Systems

**Domain Clashing**: Two active domains clash each turn. Higher `Refinement` wins outright; equal refinement goes to `Range`. Equal range is a stalemate. If three or more domains are active, all domains collapse simultaneously.

**Burnout**: Deactivating a domain burns out the technique, weakening the output for several turns. `RecoverTechniqueBurnout` ticks each end-of-turn until the technique resets to `Usable`.

**Black Flash**: Configurable per-character chance on a standard attack. On hit, it clears technique burnout, boosts technique status to `DomainBoost`, and increments a chain counter. Each consecutive Black Flash increases the multiplier through the chain counter thus increasing the damage of the next Black Flash. If the Black Flash chain breaks, the damage multiplier resets to the base 4.5× boost.

**The Zone**: Sustaining `DomainBoost` status outside an active domain grants a temporary CE regen bonus for up to 3 turns before resetting to `Usable`.

**RCT Proficiency**: Tiers from `None` → `Absolute` determine heal amount and CE cost per RCT use. `Overdrive` mode doubles both heal and cost.

**CE Reinforcement**: Reduces incoming damage by up to 3× at maximum reinforcement. Each turn, maintaining reinforcement costs `current_reinforcement × reinforcement_cost_mult` CE (default `2.0`, varies per character); if CE drops below the reinforcement value, the reinforcement collapses entirely.

**CE Efficiency**: Based on the character's Cursed Energy efficiency setting (`Wasteful`, `Rough`, `Stable`, `Excellent` or `Absolute`), it calculates how much more or less cursed energy a `CurseUser` character spends.

## Minor Systems

**Binding Vows**: Trade one resource or limitation for another advantage, such as reducing Reinforcement cost by lowering maximum Reinforcement capacity.

**Six Eyes (Sorcerers Only)**: Allows the user to perceive an opponent's technique, technique status, and cursed energy when selecting a target, while drastically lowering their own cursed energy usage.

**Passive Regeneration (Cursed Spirits/Physically Gifted Only)**: Cursed Spirits get a passive regeneration buff since they cannot use Reverse Cursed Technique. Physically Gifted characters have passive regeneration scaled by their Strength value.

---

## Project Structure

```
SorcererShowdown/
├── Core
│   ├── Character             ─ Base class: HP, cursed tools, stun, AI brain
│   ├── CurseUser             ─ CE, CE efficiency, domain/technique/shikigami management, Binding Vows, Black Flash
│   ├── Sorcerer              ─ RCT, Six Eyes
│   ├── CursedSpirit          ─ Passive HP regen per turn, no RCT. If CE is less than 5.0 the character begins to lose HP
│   ├── PhysicallyGifted      ─ Strength-based damage/defence, Heavenly Restriction
│   └── Shikigami             ─ State management: In Shadow / Active
├── Systems                 
│   ├── Techniques            ─ Base class: CalculateDamage, chant levels, status
│   ├── Domain                ─ Base class: clash resolution, surehit dispatch
│   ├── CursedTool            ─ Base tool: GetCalculatedStrength scaling
│   ├── Specials              ─ One-off special move base
│   ├── CharacterAI           ─ CharacterBrain: Aggressive / Reactive / Randomized / Brawler
│   ├── BattleManager         ─ Game loop, domain resolution, turn management
│   ├── PlayerManager         ─ Player input routing and action handling
│   └── UserInterface         ─ Status panels and action menus
├── Characters              ─ Gojo, Sukuna, Yuta, Hakari, Mahito, Toji, TransfiguredHuman
├── Techniques              ─ Limitless, Shrine, Copy, IdleTransfiguration, PrivatePureLoveTrain
├── Domains                 ─ InfiniteVoid, MalevolentShrine, AuthenticMutualLove, SimpleDomain, 
│                             IdleDeathGamble, SelfEmbodimentOfPerfection, HollowWickerBasket
│                            
├── Shikigami               ─ Mahoraga (unlocks World Cutting Slash for Shrine users), Rika (CE amplifier), Agito (Passive Healing)
├── Tools                   ─ Katana, PlayfulCloud, InvertedSpearOfHeaven, SplitSoulKatana
├── Binding Vows            ─ Resource/limitation trade-off system
├── SorcererShowdown.cpp    ─ Contains Game(), the main game loop entry point
└── main.cpp                ─ main()
```


## Building

### Requirements

- A C++23-compatible compiler:
  - **MSVC** Visual Studio 2022 v17.6+ with `/std:c++latest`
  - **Clang** 18+ with libstdc++14 or libc++
  - **GCC** 14+
- CMake 3.28+
- Internet access on first build (CMake auto-downloads `json.hpp` from the nlohmann/json repo)

### CMake

```bash
cmake -B build
cmake --build build
```

The executable lands in `build/`. If a `characters.json` exists in the project root, CMake copies it to the build directory automatically.

### Project layout expected by CMake

CMake sets the header directory as the include base, so all `#include` paths in source files are relative to it (e.g. `"Characters/Character.h"`). New `.cpp` files placed anywhere under `code/source/` are picked up automatically.

```
SorcererShowdown/
├── CMakeLists.txt
├── json.hpp                  <- auto-downloaded if missing
├── characters.json          ┐
├── cursedtools.json         ┼─> optional, copied to build directory
├── domains.json             ┘
└── code/
    ├── header/               <- all #includes are relative to this folder
    └── source/               <- all .cpp files here are compiled automatically
```

---

## How to Play

1. Select your character and opponent count
2. If no player character is chosen, **Spectator Mode** is enabled for AI vs AI
3. Choose to step or skip through AI turns
4. On your turn, pick from 11 actions: Technique actions, Attacking, Special move, Domain actions, Taunting, Reverse Cursed Technique, Cursed Tools, Technique Settings, Shikigami, CE Reinforcement and Binding Vows

---

## Modding

To learn how to mod the game, refer to [MODDING.md](MODDING.md).

## License

Fan project based on Jujutsu Kaisen by Gege Akutami. All character names and concepts belong to their respective owners.