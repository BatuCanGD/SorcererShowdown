#pragma once
class Sorcerer;
class Character;

class CursedTool {
protected:
	std::string name = "";
	std::string color = "";
	double base_damage = 10.0;
public:
	enum class WeaponType { Normal, TechniqueBypass, ReinforcementBypass, AllBypass };
	WeaponType type = WeaponType::Normal;
	virtual ~CursedTool() = default;
	virtual std::unique_ptr<CursedTool> Clone() const;
	void UseTool(Character*, Character*);
	std::string GetName() const;
	std::string GetSimpleName() const;

	virtual double GetCalculatedStrength(Character*) const;

	bool IsAntiTechniqueWeapon() const;
	void SetType(std::string_view);
	void SetName(const std::string&);
	void SetColor(const std::string&);
	void SetDamage(double);
};
