#pragma once
class Character;
class CurseUser;

class Domain {
protected:
	std::string domain_name = "";
	std::string domain_color = "";

	double domain_health;
	const double base_health;
	double domain_overwhelm_strength;
	const double base_range;
	double current_range;
	bool clashing = false;
	bool is_neutralizer = false;

	double domain_cost = 0.0;
	double surehit_damage = 0.0;

	enum class Refinement {
		Unstable, Crude, Refined, Absolute
	};
	Refinement ref_level = Refinement::Refined;
	enum class HitType {
		HitsEveryone, HitsCurseUsers
	};
	HitType hit_type = HitType::HitsCurseUsers;
public:
	virtual ~Domain();
	virtual std::unique_ptr<Domain> Clone() const = 0;
	Domain(double health, double damage, double range);
	bool Clashing() const;
	void SetClashState(bool a);
	double DomainRangeMult()const;
	virtual void OnSureHit(CurseUser& user, Character& target);
	std::string GetDomainName() const;
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
};