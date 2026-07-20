#pragma once
class Character;
class CurseUser;

class Domain {
protected:
	std::string name{};
	std::string color{};

	double domain_health{};
	double saved_health{};
	double domain_strength{};
	double domain_cost{};
	double surehit_damage{};

	int range{};
	int total_uses{};
	int cd_timer{0};
	static constexpr int cd_max{3};

	bool is_active{};
	bool on_cooldown{};
	bool is_neutralizer{};
	bool is_stunning{}; 

	virtual void DoSureHit(CurseUser& user, Character& target, bool is_blocked);
public:
	virtual ~Domain();
	virtual std::unique_ptr<Domain> Clone() const;
	Domain(double h, double dmg, int rn);

	enum class Refinement { Unstable, Crude, Refined, Absolute };
	enum class HitType { HitCurseUser, HitCurseUserSoul, HitAll, HitAllSoul };
	enum class EndReason { Manual, Auto, Expired, Collapsed, Overwhelmed };
	Refinement ref_level = Refinement::Refined;
	HitType hit_type = HitType::HitCurseUser;

	void OnSureHit(CurseUser& user, Character& target); // here
	
	std::string GetDomainName() const;
	std::string GetDomainStatus() const;
	std::string GetDomainSimpleName() const;
	double GetUseCost() const;

	double GetDomainHealth() const;
	double GetDomainStrength() const;
	int GetDomainRange() const;

	void TickDomain(CurseUser*);
	void DamageDomain(double);
	static void ClashDomains(CurseUser&, CurseUser&);
	static void ResolveRange(Domain&, Domain&, CurseUser&, CurseUser&);
	Refinement GetRefinement() const;
	HitType GetHitType() const;

	void EndDomain(CurseUser*, EndReason);
	void SetDomainActivation(CurseUser*, bool); // activate or disable domain

	bool IsActive() const;
	bool OnCooldown() const;
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