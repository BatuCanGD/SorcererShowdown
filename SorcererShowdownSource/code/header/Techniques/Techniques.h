#pragma once
#include "std.h"

class CurseUser;
class Character;
struct Battlefield;

class Technique { 
protected:
	static constexpr double base_output = 10.0;
	enum class ChantLevel {
		Zero, One, Two, 
		Three, Four
	};
	ChantLevel chant = ChantLevel::Zero;
	std::string tech_name = "";
	std::string tech_color = "";
public:
	enum class Status { Usable, DomainBoost, BurntOut };
	Status state = Status::Usable;

	std::string GetTechniqueName() const;
	std::string GetTechniqueSimpleName() const;

	virtual ~Technique() = default;
	virtual void Set(Status s);
	Status GetStatus() const;
	bool FullyChanted()const;
	double GetTechniqueOutput() const;
	double CalculateDamage(CurseUser* user, double cost) const;
	std::string GetStringStatus() const;

	virtual void Chant();
	virtual void TechniqueSetting(CurseUser*, Battlefield&);
	virtual void TechniqueMenu(CurseUser* user, Character* target, Battlefield&) = 0;
	virtual void AutoTechniqueUse(CurseUser* user, Character* target, Battlefield& bf) = 0;
	virtual std::unique_ptr<Technique> Clone() const = 0;

	std::string GetStringChantLevel() const;
	double GetChantPower() const;

	bool Boosted() const;
	bool BurntOut() const;
	bool Usable() const;

	virtual bool IsShrine() const;
	virtual bool IsLimitless() const;
	virtual bool IsInfinityActive() const;
	virtual bool IsCopy() const;
};

