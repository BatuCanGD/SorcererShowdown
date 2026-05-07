#pragma once
class Sorcerer;
class Character;

class CursedTool {
protected:
	std::string tool_name = "";
	std::string tool_color = "";

	double base_tool_damage = 20.0;
	double base_tool_support = 10.0;
	double special_tool_mult = 2.5;

	double GetCalculatedStrength(Character*) const;
public:
	virtual ~CursedTool() = default;
	virtual std::unique_ptr<CursedTool> Clone() const = 0;
	virtual void UseTool(Character*, Character*) = 0;
	std::string GetName() const;
	std::string GetSimpleName() const;

	virtual bool IsAntiTechniqueWeapon() const;
};
