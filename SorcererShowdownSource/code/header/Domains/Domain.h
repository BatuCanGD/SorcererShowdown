#pragma once
class Character;
class CurseUser;

class Domain {
protected:
	std::string name = "";
	std::string color = "";

	double domain_health;
	double saved_health;
	double domain_strength;
	double domain_cost = 0.0;
	double surehit_damage = 0.0;

	int range;
	int total_uses = 0;

	bool is_neutralizer = false;
	bool is_stunning = false; 
public:
	virtual ~Domain();
	virtual std::unique_ptr<Domain> Clone() const;
	Domain(double h, double dmg, int rn);

	enum class Refinement { Unstable, Crude, Refined, Absolute };
	enum class HitType { HitCurseUser, HitAll, HitAllSoul };

	Refinement ref_level = Refinement::Refined;
	HitType hit_type = HitType::HitCurseUser;

	virtual void OnSureHit(CurseUser& user, Character& target);
	std::string GetDomainName() const;
	std::string GetDomainSimpleName() const;
	double GetUseCost() const;

	double GetDomainHealth() const;
	double GetDomainStrength() const;
	int GetDomainRange() const;

	

	void DamageDomain(double);
	static void ClashDomains(CurseUser&, CurseUser&);
	static void ResolveRange(Domain&, Domain&, CurseUser&, CurseUser&);
	Refinement GetRefinement() const;
	HitType GetHitType() const;

	static void ResetDomain(CurseUser& user, Domain& domain);
	void CollapseDomain();

	bool IsDestroyed() const;
	bool IsNeutralizer() const;
	bool IsSurehitBlocked(Character&) const;
	virtual bool IsIdleDeathGamble()const;

	int GetDomainUses() const;	

	void IncrementUses();
	void SetDomainUses(int);
	void SetDomainStun(bool);
	void SetDomainHealth(double);
	void SetDomainName(const std::string&);
	void SetDomainColor(const std::string&);
	void SetDomainType(std::string_view);
	void SetRefinement(std::string_view);
	void SetIfDomainNeutralizer(bool);
	void SetSurehitDamage(double);
	void SetDomainCost(double);
	void SetDomainRange(int);
	void SetDomainOverwhelmStrength(double);
};