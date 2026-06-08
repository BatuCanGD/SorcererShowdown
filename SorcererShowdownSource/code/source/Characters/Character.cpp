#include "code/header/Characters/Character.h"
#include "code/header/CursedTools/CursedTool.h"
#include "code/header/Characters/CurseUsers/CurseUser.h"
#include "code/header/CharacterCreator/AI/CharacterAI.h"
#include "code/header/Characters/Shikigami/Shikigami.h"
#include "code/header/Specials/Specials.h"
#include "code/header/GameManagement/BattlefieldHeader.h"
#include "code/header/Characters/PhysicallyGifted/PhysicallyGifted.h"
#include "code/header/Techniques/Limitless/Limitless.h"
#include "code/header/Domains/Domain.h"
#include "code/header/GameManagement/Utils.h"
#include "code/header/Techniques/Techniques.h"

int Character::global_id_counter = 0;

Character::Character(double hp)
	: health(hp),max_health(hp),previous_health(hp){
}
Character::~Character() = default;

void Character::OnCharacterTurn(Battlefield& bf) {
	if (IsCharacterStunned()) {
		std::println("{} is stunned and their turn will be skipped", GetNameWithID());
		return;
	}
	if (brain) {
		brain->ExecuteTurn(this, bf);
	}
	else {
		std::println("if you see this, it means that you havent assigned"
					 "an AI type to this character, you must add it in the JSON file");
	}
}

void Character::Attack(Character* target) {
	if (cursed_tool){
		cursed_tool->UseTool(this, target);
		return;
	}
	target->Damage(attack_damage); 
}

void Character::AssignID() {
	if (CanBeAssignedID()) {
		global_id_counter++;
		unique_id = global_id_counter;
	}
}


void Character::AddToolToInventory(std::unique_ptr<CursedTool> tool) {
	if (tool) {
		inventory_curse.push_back(std::move(tool));
	}
}

void Character::Damage(double h) {
	if (!CanBeHit() || is_invulnerable) return;
	health = std::max(health - (h / GetDamageReinforcement()), 0.0);
}
void Character::DamageBypassReinforcement(double h) {
	if (is_invulnerable || !CanBeHit()) return;
	health = std::max(health - h, 0.0);
}
void Character::DamageBypass(double h) {
	if (is_invulnerable) return;
	health = std::max(health - (h / GetDamageReinforcement()), 0.0);
}
void Character::DamageBypassAll(double h) {
	if (is_invulnerable) return;
	health = std::max(health - h, 0.0);
}

void Character::ClearStunTime() {
	stun_duration--;
	if (stun_duration <= 0) {
		is_stunned = false;
		stun_duration = 3;
	}
}

void Character::CursedToolChoice(size_t choice) {
	if (choice == 0) {
		if (cursed_tool != nullptr) {
			std::println("{}{} put {} away.{}", Utilities::Color::BrightRed, GetNameWithID(), cursed_tool->GetName(), Utilities::Color::Clear);
			inventory_curse.push_back(std::move(cursed_tool));
			cursed_tool = nullptr;
		}
		return;
	}

	size_t inv_index = choice - 1;
	if (inv_index < inventory_curse.size()) {
		if (cursed_tool != nullptr) {
			inventory_curse.push_back(std::move(cursed_tool));
		}
		std::swap(inventory_curse[inv_index], inventory_curse.back());
		cursed_tool = std::move(inventory_curse.back());
		inventory_curse.pop_back();
		std::println("{}{} equipped {}!{}", Utilities::Color::Cyan, GetNameWithID(), cursed_tool->GetName(), Utilities::Color::Clear);
	}
	else {
		std::println("{}Invalid tool choice.{}", Utilities::Color::Red, Utilities::Color::Clear);
	}
}

void Character::EquipToolByName(const std::string& weaponName) {
	for (size_t i = 0; i < inventory_curse.size(); ++i) {
		if (inventory_curse[i]->GetSimpleName() == weaponName) {
			CursedToolChoice(i + 1);
			return;
		}
	}
}

void Character::TickCharacterSpecialty() {}

void Character::Taunt(Character* taunted) const {
	if (!taunted) return;
    double hp_ratio = health / max_health;
    std::string target = taunted->GetNameWithID();
    int type = Utilities::GetRandomNumber(1, 4);

    if (hp_ratio > 0.70) {
        switch (type) {
            case 1: std::println("Stand proud, {}. You're strong.", target); break;
            case 2: std::println("Are you the challenger because you're {}, or are you {} because you're the challenger?", target, target); break;
            case 3: std::println("You're just a nameless fish on my cutting board, {}.", target); break;
            default: std::println("You really thought you were the one who would win, {}?", target);
        }
    }
    else if (hp_ratio > 0.50) {
        switch (type) {
            case 1: std::println("You're starting to see it, aren't you, {}? The core of Cursed Energy!", target); break;
            case 2: std::println("I'll have to adjust my evaluation of you, {}.", target); break;
            case 3: std::println("Don't get cocky, {}. You haven't even forced me to use my technique yet.", target); break;
            default: std::println("You're actually making me work for this, {}.", target);
        }
    }
    else if (hp_ratio > 0.25) {    
        switch (type) {
            case 1: std::println("Is this the 'spark' you were hoping for, {}?", target); break;
            case 2: std::println("This is true Sorcery! Don't you dare look away, {}!", target); break;
            case 3: std::println("You've peeled back my scales, {}. Now see what's underneath!", target); break;
            default: std::println("You're really pushing your luck now, {}!", target);
        }
    }
    else {
        switch (type) {
            case 1: std::println("Even compared to you, {}, I alone am the honored one.", target); break;
            case 2: std::println("Even if I die, my curse will haunt you forever, {}!", target); break;
            case 3: std::println("To merit the title of 'Sorcerer' you'll have to do better than that, {}.", target); break;
            default: std::println("You're just a monkey who can't even finish the job, {}!", target);
        }
    }
}


std::string Character::GetNameWithID()const { return std::format("{}{}{} ({})",color ,name, Utilities::Color::Clear, unique_id); }
std::string Character::GetName() const { return std::format("{}{}{}", color, name, Utilities::Color::Clear); }
std::string Character::GetSimpleName() const { return name; }
std::string Character::GetType() const{ return character_type; }

bool Character::IsaSorcerer() const { return false; }
bool Character::IsShikigami() const { return false; }
bool Character::IsPhysicallyGifted() const { return false; }
bool Character::IsaCurseUser() const { return false; }
bool Character::IsaCursedSpirit() const { return false; }
bool Character::CanBeAssignedID() const { return true; }
bool Character::IsThePlayer() const { return is_player; }
bool Character::CanBeHit() const { return true; }
bool Character::IsCharacterStunned() const { return is_stunned; }
bool Character::HPMoreThanMax(double h) const { return GetCharacterHealth() > GetCharacterMaxHealth() * h; }

double Character::GetDamageReinforcement()const { return 1.0; }
double Character::GetBaseAttackDamage() const { return attack_damage; }
double Character::GetCharacterHealth() const { return health; }
double Character::GetCharacterMaxHealth() const { return max_health; }
double Character::GetCharacterPreviousHealth() const { return previous_health; }

void Character::SetEquippedTool(std::unique_ptr<CursedTool> tool) { cursed_tool = std::move(tool); }
void Character::SetBrain(std::unique_ptr<CharacterBrain> new_brain) { brain = std::move(new_brain); }
void Character::SetAsPlayer(bool p) { is_player = p; }
void Character::SetVulnerability(bool t) { is_invulnerable = t; }
void Character::SetHealth(double h) { health = h; }
void Character::SetCharacterName(std::string n) { name = n; }
void Character::SetCharacterColor(std::string c) { color = c; }
void Character::SetBaseDamage(double d) { attack_damage = d; }
void Character::Regen(double h) { health = std::min(health + h, max_health); }
void Character::SetStunState(bool s) { is_stunned = s; }
void Character::UpdatePreviousHP() { previous_health = health; }

int Character::GetID() const { return unique_id; }
CursedTool* Character::GetTool() const { return cursed_tool.get(); }
const std::vector<std::unique_ptr<CursedTool>>& Character::GetCursedTools() const { return inventory_curse; }

std::unique_ptr<Character> Character::Clone() const {
    auto s = std::make_unique<Character>(max_health);
    if (brain) s->SetBrain(brain->Clone());
    s->SetCharacterName(name);
    s->SetCharacterColor(color);
    s->SetBaseDamage(attack_damage);
    for (const auto& tool : inventory_curse) {
        if (tool) s->AddToolToInventory(tool->Clone());
    }
    return s;
} 