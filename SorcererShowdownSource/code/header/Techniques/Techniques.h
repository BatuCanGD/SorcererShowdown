#pragma once
class CurseUser;
class Character;
struct Battlefield;

class Technique { 
protected:
	std::string name;
	std::string color;
public:
	virtual ~Technique();
	
	enum class ChantLevel { Zero, One, Two, Three, Four };
	enum class Status { Usable, DomainBoost, BurntOut };
	Status state = Status::Usable;
	ChantLevel chant = ChantLevel::Zero;

	std::string GetTechniqueName() const;
	std::string GetTechniqueSimpleName() const;
	
	virtual void Set(Status s);
	Status GetStatus() const;
	ChantLevel& GetChantLevel();
	bool FullyChanted()const;
	double GetTechniqueOutput() const;
	std::string GetStringStatus() const;

	virtual void Chant();
	virtual void TechniqueSetting(CurseUser*, Battlefield&);
	virtual void TechniqueMenu(CurseUser* user, Character* target, Battlefield&);
	virtual bool AutoTechniqueUse(CurseUser* user, Character* target, Battlefield& bf);
	virtual std::unique_ptr<Technique> Clone() const;

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

