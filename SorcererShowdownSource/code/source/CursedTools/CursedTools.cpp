#include "code/header/CursedTools/CursedTool.h"
#include "code/header/Characters/PhysicallyGifted/PhysicallyGifted.h"
#include "code/header/Characters/CurseUsers/CurseUser.h"
#include "code/header/GameManagement/Utils.h"


double CursedTool::GetCalculatedStrength(Character* user)  const {
	if (user->IsPhysicallyGifted()) { 
		return base_damage + (static_cast<PhysicallyGifted*>(user)->GetStrengthDamage() * 2.5);
	}
	else {
		return base_damage + ((std::sqrt(static_cast<CurseUser*>(user)->GetCharacterCE()) * 1.5) * 0.67);
	}
}

std::string CursedTool::GetName() const {
	return std::format("{}{}{}", color, name, Utilities::Color::Clear);
}

std::string CursedTool::GetSimpleName()const {
	return name;
}

void CursedTool::UseTool(Character* user, Character* target){
	bool bypass_weapon = type == WeaponType::AllBypass || type == WeaponType::TechniqueBypass;
	double damage = GetCalculatedStrength(user);
	if (target->IsaCurseUser()){
		auto crs = static_cast<CurseUser*>(target);
		if (crs->GetTechnique() && crs->GetTechnique()->IsLimitless() && crs->GetTechnique()->IsInfinityActive()){
			if (bypass_weapon){
				target->DamageBypass(damage);
				return;
			}else{
				std::println("The Cursed Tool was blocked by {}'s infinity!", target->GetNameWithID());
				return;
			}
		}
	}
	if (type == WeaponType::ReinforcementBypass){
		target->DamageBypassReinforcement(damage);
		std::println("{} attacked {}'s soul with {}", user->GetNameWithID(), target->GetNameWithID(), this->GetName());
		return;
	}
	target->Damage(damage);
	std::println("{} attacked {} with {}", user->GetNameWithID(), target->GetNameWithID(), this->GetName());
}

void CursedTool::SetType(const std::string& s){
	if (s == "Normal"){
		type = WeaponType::Normal;
	}else if (s == "Bypass Techniques"){
		type = WeaponType::TechniqueBypass;
	}else if (s == "Bypass Reinforcement"){
		type = WeaponType::ReinforcementBypass;
	}else if (s == "Bypass Everything"){
		type = WeaponType::AllBypass;
	}else{
		type = WeaponType::Normal;
	}
}
void CursedTool::SetName(const std::string& s){
	name = s;
}
void CursedTool::SetColor(const std::string& s){
	color = s;
}
void CursedTool::SetDamage(double d){
	base_damage = d;
}

bool CursedTool::IsAntiTechniqueWeapon() const {
	return type == WeaponType::AllBypass || type == WeaponType::TechniqueBypass;
}

std::unique_ptr<CursedTool> CursedTool::Clone() const{
	return std::make_unique<CursedTool>(*this);
}

