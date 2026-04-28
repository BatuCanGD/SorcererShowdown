#include "Character.h"
#include "Shikigami.h"
#include "Specials.h"
#include "BattlefieldHeader.h"
#include "PhysicallyGifted.h"
#include "Limitless.h"
#include "Domain.h"
#include "Utils.h"
#include "CursedTool.h"
#include "Sorcerer.h"
#include "Techniques.h"

import std;

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

	auto* cu = dynamic_cast<CurseUser*>(this);
	auto* sr = dynamic_cast<Sorcerer*>(this);

	CharacterAI ai = this->GetCustomAI();

	Character* target = nullptr;
	double best_score = -1.0;
	std::vector<CurseUser*> domain_users;

	for (const auto& entity : bf.battlefield) {
		if (entity.get() == this || entity->GetCharacterHealth() <= 0) continue;

		double score = 0.0;
		double hp_ratio = entity->GetCharacterHealth() / entity->GetCharacterMaxHealth();

		switch (ai) {
		case CharacterAI::Aggressive: score = hp_ratio;       break;
		case CharacterAI::Reactive:   score = 1.0 - hp_ratio; break;
		}

		if (auto* target_cu = dynamic_cast<CurseUser*>(entity.get())) {
			if (target_cu->DomainActive()) {
				domain_users.push_back(target_cu);
				score += 0.50;
			}
			if (target_cu->GetTechnique()) {
				std::string tech = target_cu->GetTechnique()->GetTechniqueSimpleName();
				if (tech == "Shrine")    score += 1.0;
				if (tech == "Limitless") score += 0.15;
			}
		}
		else if (dynamic_cast<PhysicallyGifted*>(entity.get())) {
			score += 0.25;
		}

		score += GetRandomNumber(-5, 5) * 0.025;

		if (score > best_score) {
			best_score = score;
			target = entity.get();
		}
	}

	if (!target) return;
	bool target_has_infinity = false;
	if (auto cr = dynamic_cast<CurseUser*>(target)) {
		if (cr->GetTechnique()) {
			if (auto lim = dynamic_cast<Limitless*>(cr->GetTechnique())) {
				if (lim->CheckInfinity()) target_has_infinity = true;
			}
		}
	}

	if (GetRandomNumber(1, 10) <= 3) {
		this->Taunt(target);
	}

	if (cu) {
		if (sr) {
			switch (ai) {
			case CharacterAI::Aggressive:
				if (!this->HPMoreThanMax(0.25) && cu->CEMoreThanMax(0.20)) sr->BoostRCT();
				else if (!this->HPMoreThanMax(0.60) && cu->CEMoreThanMax(0.10))  sr->EnableRCT();
				else sr->DisableRCT();
				break;
			case CharacterAI::Reactive:
				if (!this->HPMoreThanMax(0.35) && cu->CEMoreThanMax(0.15)) sr->BoostRCT();
				else if (!this->HPMoreThanMax(0.65) && cu->CEMoreThanMax(0.10)) sr->EnableRCT();
				else sr->DisableRCT();
				break;
			}
		}

		switch (ai) {
		case CharacterAI::Aggressive:
			if (cu->CEMoreThanMax(0.50))      cu->SetCurrentReinforcement(200.0);
			else if (cu->CEMoreThanMax(0.30)) cu->SetCurrentReinforcement(100.0);
			else if (cu->CEMoreThanMax(0.20)) cu->SetCurrentReinforcement(50.0);
			else                              cu->SetCurrentReinforcement(0.0);
			break;
		case CharacterAI::Reactive:
			if (!this->HPMoreThanMax(0.35))   cu->SetCurrentReinforcement(200.0);
			else if (cu->CEMoreThanMax(0.40)) cu->SetCurrentReinforcement(100.0);
			else                              cu->SetCurrentReinforcement(0.0);
			break;
		}

		if (!cu->GetShikigami().empty()) {
			for (const auto& shiki : cu->GetShikigami()) {
				if (!shiki->IsActive()) {
					if (cu->CEMoreThanMax(0.30)) shiki->Manifest();
				}
				else if (shiki->IsActivePhysically() && !cu->CEMoreThanMax(0.15)) {
					shiki->Withdraw();
				}
			}
		}

		if (!domain_users.empty()) {
			switch (ai) {
			case CharacterAI::Aggressive:
				if (cu->GetDomain() && cu->GetCharacterCE() >= cu->GetDomain()->GetUseCost()
					&& !cu->DomainActive() && !cu->IsStrained() && cu->GetDomainUses() < 5) {
					if (!cu->GetTechnique() || !cu->GetTechnique()->BurntOut()) {
						cu->ActivateDomain();
						return;
					}
				}
				if (cu->GetCounterDomain() && !cu->CounterDomainActive()) {
					cu->ActivateCounterDomain();
					return;
				}
				break;
			case CharacterAI::Reactive:
				if (cu->GetCounterDomain() && !cu->CounterDomainActive()) {
					cu->ActivateCounterDomain();
					return;
				}
				break;
			}
		}
		else {
			if (cu->CounterDomainActive()) cu->DeactivateCounterDomain();

			switch (ai) {
			case CharacterAI::Aggressive:
				if (cu->GetDomain() && !cu->DomainActive() && !cu->IsStrained()
					&& cu->GetDomainUses() < 5 && GetRandomNumber(1, 100) <= 25) {
					if (!cu->GetTechnique() || !cu->GetTechnique()->BurntOut()) {
						cu->ActivateDomain();
						return;
					}
				}
				break;
			case CharacterAI::Reactive:
				if (cu->GetDomain() && !cu->DomainActive() && !cu->IsStrained()
					&& cu->GetDomainUses() < 5 && !this->HPMoreThanMax(0.40)) {
					if (!cu->GetTechnique() || !cu->GetTechnique()->BurntOut()) {
						cu->ActivateDomain();
						return;
					}
				}
				break;
			}
		}

		if (cu->GetSpecial() && cu->CEMoreThanMax(0.35) && GetRandomNumber(1, 100) <= 20) {
			cu->GetSpecial()->PerformSpecial(cu);
			return;
		}

		if (target_has_infinity) cu->SetAmplification(true);
		else if (cu->DomainAmplificationActive()) cu->SetAmplification(false);

		if (cu->GetTechnique() && !cu->GetTechnique()->BurntOut()
			&& (cu->GetTechnique()->Usable() || cu->GetTechnique()->Boosted())
			&& !cu->DomainAmplificationActive()) {

			switch (ai) {
			case CharacterAI::Aggressive:
				if (cu->CEMoreThanMax(0.20)) {
					cu->GetTechnique()->AutoTechniqueUse(cu, target, bf);
					return;
				}
				break;
			case CharacterAI::Reactive:
				if (!this->HPMoreThanMax(0.50) || cu->GetTechnique()->Boosted()) {
					cu->GetTechnique()->AutoTechniqueUse(cu, target, bf);
					return;
				}
				break;
			}
		}
	}
	if (target_has_infinity) {
		if (!cursed_tool || cursed_tool->GetSimpleName() != "The Inverted Spear of Heaven") {
			this->EquipToolByName("The Inverted Spear of Heaven");
			return;
		}
	}
	else if (!inventory_curse.empty() && !cursed_tool) {
		if (GetRandomNumber(1, 100) <= 50) {
			CursedToolChoice(static_cast<size_t>(GetRandomNumber(1, static_cast<int>(inventory_curse.size())))); 
			return;
		}
	}
	else if (cursed_tool && !inventory_curse.empty()) {
		int roll = GetRandomNumber(1, 100);
		if (roll <= 5) {
			CursedToolChoice(size_t(0));
			return;
		}
		else if (roll <= 20) {
			CursedToolChoice(static_cast<size_t>(GetRandomNumber(1, static_cast<int>(inventory_curse.size()))));
			return;
		}
	}

	this->Attack(target);
	if (cu && cu->DomainAmplificationActive()) cu->SetAmplification(false);
}

void Character::SetAIType(const std::string& str) {
	if (str == "Reactive") ai_type = CharacterAI::Reactive;
	else if (str == "Aggressive") ai_type = CharacterAI::Aggressive;
	else ai_type = CharacterAI::Aggressive;
}

bool Character::CanBeAssignedID() const {
	return true;
}

void Character::Attack(Character* target) {
	if (!target || target->GetCharacterHealth() <= 0) return;

	CurseUser* me_cuser = dynamic_cast<CurseUser*>(this);
	CurseUser* target_cuser = dynamic_cast<CurseUser*>(target);
	PhysicallyGifted* me_gifted = dynamic_cast<PhysicallyGifted*>(this);

	if (target_cuser) {
		if (auto* tech = target_cuser->GetTechnique()) {
			if (auto* limitless = dynamic_cast<Limitless*>(tech)) {
				bool has_da = me_cuser && me_cuser->DomainAmplificationActive();
				if (limitless->CheckInfinity() && !has_da) {
					std::println("{}'s strike was slowed to a halt by {}'s {}Infinity{}!",
						this->GetNameWithID(), target_cuser->GetNameWithID(), Color::Cyan, Color::Clear);
					return;
				}
			}
		}
	}
	if (me_cuser && me_cuser->DomainAmplificationActive()) {
		double ce_addon = std::sqrt(std::max(0.0, me_cuser->GetCharacterCE())) * 0.888;
		double amp_damage = this->base_attack_damage + ce_addon;

		target->DamageBypass(amp_damage); 
		std::println("{} neutralizes {}'s Infinity and hits them using Domain Amplification!",
			this->GetNameWithID(), target->GetNameWithID());
		return;
	}
	if (cursed_tool) {
		cursed_tool->UseTool(this, target);
		return;
	}

	double current_base = this->base_attack_damage;
	double multiplier = 1.0;
	bool is_black_flash = false;

	if (me_cuser) {
		if (GetRandomNumber(1, 100) <= me_cuser->GetBlackFlashChance()) {
			is_black_flash = true;
			multiplier = 4.0;

			if (auto* tech = me_cuser->GetTechnique()) {
				tech->Set(Technique::Status::DomainBoost);
			}
			me_cuser->TickZone();
		}
	}
	else if (me_gifted) {
		current_base += me_gifted->GetStrengthDamage();
	}

	target->Damage(current_base * multiplier);

	if (is_black_flash) {
		std::println("\n{}*** {}BLACK FLASH!{} ***{}", Color::BrightMagenta, Color::Red, Color::BrightMagenta, Color::Clear);
		std::println("The space distorts! {} lands a devastating blow on {}!",
			this->GetNameWithID(), target->GetNameWithID());
	}
	else {
		std::println("{} lands a {}solid strike{} on {}!",
			this->GetNameWithID(), Color::BrightRed, Color::Clear, target->GetNameWithID());
	}
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
	for (int i = 0; i < inventory_curse.size(); ++i) {
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

Character::CharacterAI Character::GetCustomAI() const {
	return ai_type;
}

void Character::SetEquippedTool(std::unique_ptr<CursedTool> tool) {
	cursed_tool = std::move(tool);
}