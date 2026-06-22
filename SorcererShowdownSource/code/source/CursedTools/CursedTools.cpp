#include "code/header/CursedTools/CursedTool.h"
#include "code/header/Characters/PhysicallyGifted/PhysicallyGifted.h"
#include "code/header/Characters/CurseUsers/CurseUser.h"
#include "code/header/GameManagement/Colors.h"


double CursedTool::GetCalculatedStrength(Character* user)  const {
	if (user->IsPhysicallyGifted()) { 
		return base_damage + (static_cast<PhysicallyGifted*>(user)->GetStrengthDamage() * 2.5);
	}
	else {
		return base_damage + ((std::sqrt(static_cast<CurseUser*>(user)->GetCharacterCE()) * 1.33) * 0.67);
	}
}

std::string CursedTool::GetName() const {
	return std::format("{}{}{}", color, name, Color::Clear);
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
				if (type == WeaponType::AllBypass) target->DamageBypassAll(damage);
				else target->DamageBypass(damage);
				std::println("{} bypassed {}'s infinity with {}!",user->GetNameWithID(), target->GetNameWithID(), GetName());
				return;
			}else{
				std::println("{} was blocked by {}'s infinity!",GetName(), target->GetNameWithID());
				return;
			}
		}
	}
	if (type == WeaponType::ReinforcementBypass){
		target->DamageBypassReinforcement(damage);
		std::println("{} attacked {}'s soul with {}", user->GetNameWithID(), target->GetNameWithID(), GetName());
		return;
	}
	else if (type == WeaponType::AllBypass){
		target->DamageBypassAll(damage);
		std::println("{} attacked {}'s soul with {}", user->GetNameWithID(), target->GetNameWithID(), GetName());
		return;
	}
	target->Damage(damage);
	std::println("{} attacked {} with {}", user->GetNameWithID(), target->GetNameWithID(), GetName());
}

void CursedTool::SetType(std::string_view s){
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

