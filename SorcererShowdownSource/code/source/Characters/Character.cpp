#include "code/header/Characters/Character.h"
#include "code/header/CursedTools/CursedTool.h"
#include "code/header/Characters/CurseUsers/CurseUser.h"
#include "code/header/CharacterCreator/AI/CharacterAI.h"
#include "code/header/Characters/Shikigami/Shikigami.h"
#include "code/header/Specials/Specials.h"
#include "code/header/GameManagement/BattlefieldHeader.h"
#include "code/header/Characters/PhysicallyGifted/PhysicallyGifted.h"
#include "code/header/Techniques/Limitless.h"
#include "code/header/Domains/Domain.h"
#include "code/header/GameManagement/Utils.h"
#include "code/header/Techniques/Techniques.h"




int Character::global_id_counter = 0;

Character::Character(double hp)
	: health(hp),max_health(hp),previous_health(hp){
}
Character::~Character() = default;

void Character::OnCharacterTurn(Character*, Battlefield& bf) {
	if (this->IsCharacterStunned()) {
		std::println("{} is stunned and their turn will be skipped", this->GetNameWithID());
		return;
	}
	if (brain) {
		brain->ExecuteTurn(this, bf);
	}
	else {
		std::println("if you see this, it means that you havent assigned"
					 "an AI type to this character, you must add it");
	}
}

bool Character::CanBeAssignedID() const {
	return true;
}

void Character::Attack(Character*) {
	std::println("this is not supposed to be reached"
				 "i realized it way too late"
				 "characters with their chosen types use their own attack function");
}

void Character::AssignID() {
	if (CanBeAssignedID()) {
		global_id_counter++;
		unique_id = global_id_counter;
	}
}

bool Character::IsThePlayer() const {
	return is_player;
}

void Character::SetAsPlayer(bool p) {
	is_player = p;
}

void Character::SetVulnerability(bool t) {
	is_invulnerable = t;
}

double Character::GetBaseAttackDamage() const {
	return base_attack_damage;
}

void Character::SetHealth(double h) {
	health = h;
}

void Character::SetCharacterName(std::string name, std::string color) {
	char_name = name;
	name_color = color;
}

void Character::AddToolToInventory(std::unique_ptr<CursedTool> tool) {
	if (tool) {
		inventory_curse.push_back(std::move(tool));
	}
}

bool Character::CanBeHit() const {
	return true;
}

std::unique_ptr<Character> Character::Clone() const {
	return nullptr;
}

double Character::GetDamageReinforcement()const { return 1.0f; }

void Character::Damage(double h) {
	if (!CanBeHit() || is_invulnerable) return;
	if (this->IsaCurseUser()) { auto crs = static_cast<CurseUser*>(this);
		health = std::max(health - (h / crs->GetDamageReinforcement()), 0.0);
		return;
	}
	health = std::max(health - h, 0.0);
}
void Character::DamageBypass(double h) {
	if (is_invulnerable) return;
	if (this->IsaCurseUser()) { auto crs = static_cast<CurseUser*>(this);
		health = std::max(health - (h / crs->GetDamageReinforcement()), 0.0);
		return;
	}
	health = std::max(health - h, 0.0);
}

void Character::SetBaseDamage(double d) {
	base_attack_damage = d;
}

void Character::Regen(double h) {
	health = std::min(health + h, max_health);
}

double Character::GetCharacterHealth() const {
    return health;
}
double Character::GetCharacterMaxHealth() const {
	return max_health;
}

double Character::GetCharacterPreviousHealth() const {
	return previous_health;
}

void Character::SetStunState(bool s) {
	is_stunned = s;
}

void Character::UpdatePreviousHP() {
	previous_health = health;
}

bool Character::IsCharacterStunned() const {
	return is_stunned;
}

bool Character::HPMoreThanMax(double h) const {
	return this->GetCharacterHealth() > this->GetCharacterMaxHealth() * h;
}

std::string Character::GetName() const {
	return std::format("{}{}{}", name_color, char_name, Color::Clear);
}
std::string Character::GetSimpleName() const {
	return char_name;
}

void Character::ClearStunTime() {
	if (stun_duration <= 0) {
		is_stunned = false;
		stun_duration = 2;
	}
	else {
		stun_duration--;
	}
}

int Character::GetID() const {
	return unique_id;
}

std::string Character::GetNameWithID()const {
	return std::format("{}{}{} ({})",name_color ,char_name, Color::Clear, unique_id);
}

bool Character::IsaSorcerer() const {
	return false;
}
bool Character::IsShikigami() const {
	return false;
}
bool Character::IsPhysicallyGifted() const {
	return false;
}
bool Character::IsaCurseUser() const {
	return false;
}
bool Character::IsaCursedSpirit() const {
	return false;
}

void Character::CursedToolChoice(size_t choice) {
	if (choice == 0) {
		if (cursed_tool != nullptr) {
			std::println("{}{} put {} away.{}", Color::BrightRed, this->GetNameWithID(), cursed_tool->GetName(), Color::Clear);
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
		cursed_tool = std::move(inventory_curse[inv_index]);
		inventory_curse.erase(inventory_curse.begin() + inv_index);

		std::println("{}{} equipped {}!{}", Color::Cyan, this->GetNameWithID(), cursed_tool->GetName(), Color::Clear);
	}
	else {
		std::println("{}Invalid tool choice.{}", Color::Red, Color::Clear);
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

void Character::TickCharacterSpecialty() {};

const std::vector<std::unique_ptr<CursedTool>>& Character::GetCursedTools() const {
	return inventory_curse;
}

CursedTool* Character::GetTool() const {
	return cursed_tool.get();
}

void Character::Taunt(Character* taunted) const { // pure aura
	if (!taunted) return;
	const double healthy_threshold = 0.70;
	const double injured_threshold = 0.40;
	const double critical_threshold = 0.20;

	int taunt_type = GetRandomNumber(1, 4);

	if (this->HPMoreThanMax(healthy_threshold)) {
		switch (taunt_type) {
		case 1:
			std::println("I'm surprised you've even managed to scratch me this much {}!", taunted->GetNameWithID());
			break;
		case 2:
			std::println("Is that all you've got, {}? I expected more from you!", taunted->GetNameWithID());
			break;
		case 3:
			std::println("You're not even worth my time, {}!", taunted->GetNameWithID());
			break;
		default:
			std::println("You should just give up now, {}!", taunted->GetNameWithID());
		}
	}
	else if (this->HPMoreThanMax(injured_threshold)) {
		switch (taunt_type) {
		case 1:
			std::println("You're starting to annoy me, {}. Keep it up and I'll make you regret it!", taunted->GetNameWithID());
			break;
		case 2:
			std::println("You're not doing too bad, {}. But don't get too confident just yet!", taunted->GetNameWithID());
			break;
		case 3:
			std::println("Huh, you're actually putting up a fight, {}. I might have to take you more seriously!", taunted->GetNameWithID());
			break;
		default:
			std::println("You're not bad, {}. But I'm still better!", taunted->GetNameWithID());

		}
	}
	else if (this->HPMoreThanMax(critical_threshold)) {
		switch (taunt_type) {
		case 1:
			std::println("You're really starting to piss me off, {}. I'll make you regret your actions!", taunted->GetNameWithID());
			break;
		case 2:
			std::println("You're actually pretty strong, but it won't be enough to defeat me, {}!", taunted->GetNameWithID());
			break;
		case 3:
			std::println("You're really starting to get on my nerves, {}. I might have to end this quickly!", taunted->GetNameWithID());
			break;
		default:
			std::println("You think a few hits will stop me, {}? Im just getting warmed up!", taunted->GetNameWithID());
		}
	}
	else {
		switch (taunt_type) {
		case 1:
			std::println("You think this is over, {}? I'll drag you to the grave with me!", taunted->GetNameWithID());
			break;
		case 2:
			std::println("Blood for blood, {}! You won't leave this place alive!", taunted->GetNameWithID());
			break;
		case 3:
			std::println("You're really starting to piss me off, {}. I might have to end this quickly!", taunted->GetNameWithID());
			break;
		default:
			std::println("I'll make you wish you were never born {}!", taunted->GetNameWithID());
		}
	}
}
void Character::SetEquippedTool(std::unique_ptr<CursedTool> tool) {
	cursed_tool = std::move(tool);
}
void Character::SetBrain(std::unique_ptr<CharacterBrain> new_brain) {
	brain = std::move(new_brain);
}