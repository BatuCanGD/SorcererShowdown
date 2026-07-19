#pragma once
class CurseUser;
class Character;
struct Battlefield;

class Technique {
protected:
	std::string name;
	std::string color;

	int cd_timer{};
	int cd_max{3};

	double CalculateOutput(CurseUser* user, double output) const;
public:
	virtual ~Technique();

	enum class ChantLevel { Zero, One, Two, Three, Four };
	ChantLevel chant = ChantLevel::Zero;
	enum class Status { Usable, DomainBoost, BurntOut };
	Status state = Status::Usable;
	
	std::string GetTechniqueName() const;
	std::string GetTechniqueSimpleName() const;

	virtual void Set(Status s);
	Status GetStatus() const;
	ChantLevel& GetChantLevel();
	bool FullyChanted()const;
	double GetTechniqueOutput() const;
	std::string GetStringStatus() const;

	virtual void Chant();
	virtual bool TechniqueSetting(CurseUser*, Battlefield&);
	virtual bool TechniqueMenu(CurseUser* user, Character* target, Battlefield&);
	virtual bool AutoTechniqueUse(CurseUser* user, Character* target, Battlefield& bf);
	virtual std::unique_ptr<Technique> Clone() const;

	virtual bool HasInvulnerabilityBarrier() const;
	virtual void SetInvulnerabilityBarrier(bool);

	void InvulnerabilityNerf(CurseUser* user);

	std::string GetStringChantLevel() const;
	double GetChantPower() const;

	void TickTechnique(CurseUser*);

	bool Boosted() const;
	bool BurntOut() const;
	bool Usable() const;

	virtual bool IsShrine() const;
	virtual bool IsLimitless() const;
	virtual bool IsCopy() const;
};
