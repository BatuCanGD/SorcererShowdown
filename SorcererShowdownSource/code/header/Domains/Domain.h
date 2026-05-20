#pragma once
class Character;
class CurseUser;

class Domain {
protected:
	std::string name = "";
	std::string color = "";

	double domain_health;
	double base_health;
	double domain_overwhelm_strength;
	double base_range;
	double current_range;
	bool clashing = false;
	bool is_neutralizer = false;

	bool is_stunning = false; // for custom domains using the default onsurehit function
	// custom domains can also be set as neutralizer functions but it wont trigger the onsurehit function

	double domain_cost = 0.0;
	double surehit_damage = 0.0;
public:
	virtual ~Domain();
	virtual std::unique_ptr<Domain> Clone() const;
	Domain(double health, double damage, double range);

	enum class Refinement { Unstable, Crude, Refined, Absolute };
	enum class HitType { HitCurseUser, HitAll, HitAllSoul };

	Refinement ref_level = Refinement::Refined;
	HitType hit_type = HitType::HitCurseUser;

	bool Clashing() const;
	void SetClashState(bool a);
	double DomainRangeMult()const;
	virtual void OnSureHit(CurseUser& user, Character& target);
	std::string GetDomainName() const;
	std::string GetDomainSimpleName() const;
	double GetUseCost() const;

	double GetDomainHealth() const;
	double GetDomainStrength() const;
	double GetDomainRange() const;

	bool IsSurehitBlocked(Character&) const;

	void DamageDomain(double);
	static void ClashDomains(CurseUser&, CurseUser&);
	Refinement GetRefinement() const;
	HitType GetHitType() const;

	static void KillSetDomain(CurseUser& user, Domain& domain);
	void CollapseDomain();

	bool IsDestroyed() const;
	bool IsNeutralizer() const;

	virtual bool IsIdleDeathGamble()const;

	void SetDomainStun(bool);
	void SetDomainHealth(double);
	void SetDomainName(const std::string&);
	void SetDomainColor(const std::string&);
	void SetDomainType(const std::string&);
	void SetRefinement(const std::string&);
	void SetIfDomainNeutralizer(bool);
	void SetSurehitDamage(double);
	void SetDomainCost(double);
	void SetDomainRange(double);
	void SetDomainOverwhelmStrength(double);
};