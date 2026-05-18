#pragma once
class Sorcerer;
class Character;

class CursedTool {
protected:
	std::string name = "";
	std::string color = "";
	double base_damage = 10.0;
	double GetCalculatedStrength(Character*) const;
public:
	virtual ~CursedTool() = default;
	virtual std::unique_ptr<CursedTool> Clone() const = 0;
	virtual void UseTool(Character*, Character*) = 0;
	std::string GetName() const;
	std::string GetSimpleName() const;

	virtual bool IsAntiTechniqueWeapon() const;
};
