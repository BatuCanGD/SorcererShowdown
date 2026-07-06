#pragma once
#include "code/header/Characters/Character.h"
#include "code/header/Domains/Domain.h"
#include "code/header/Techniques/Techniques.h"
#include "code/header/Specials/Specials.h"
#include "code/header/Characters/Shikigami/Shikigami.h"
#include "code/header/BindingVows/BindingVows.h"

class CurseUser : public Character {
protected:
	std::vector<std::unique_ptr<Shikigami>> shikigami;
    std::vector<std::unique_ptr<BindingVow>> binding_vows;
    std::unique_ptr<Domain> domain;
    std::unique_ptr<Domain> counter_domain;
    std::unique_ptr<Technique> technique;
    std::unique_ptr<Specials> special;

    double cursed_energy{};
    double max_cursed_energy{};
    double saved_max_cursed_energy{};
    double prev_cursed_energy{};
    double ce_regen{};
    double saved_ce_regen{};
    double current_ce_reinforcement;
    double max_reinforcement;
    double reinforcement_cost_mult{2.0};
    double blackflash_mult{4.5};

    int technique_burnout_time{};
    int burnout_time{};
    int max_technique_burnout_time{4};
    int active_domain_time{};
    int max_domain_time{5};
    int domain_limit{5};
    int active_counter_time{}; 
    int max_counter_time{3};
    int max_counter_cooldown{2};
    int counter_recover_time{};
    int the_zone_time{};
    int max_zone_time{3};
    int black_flash_chance{5};  
    int blackflash_chain{};

    bool domain_amplification_active{};
    bool counter_domain_active{};
    bool domain_active{};
    bool is_strained{};
    bool counter_on_cooldown{};
    bool zone_ce_boost{};
public:
    CurseUser(double hp, double ce, double regen);
    ~CurseUser() override;

	Technique* GetTechnique() const;
	Specials* GetSpecial() const;
	Domain* GetCounterDomain() const;
	Domain* GetDomain() const;

	std::string GetDAstatus() const;
	std::string GetDomainStatus() const;
	std::string GetCounterStatus() const;
	std::string GetReinforcementStatus() const;
	std::string GetType() const override;

	virtual void SpendCE(double ce);
	void SetCursedEnergy(double c);
	void SetMaxCursedEnergy(double c);
	void SetCursedEnergyRegen(double c);
	void RegenCE();

	bool CEMoreThanMax(double) const;

	double GetCursedEnergyRegen() const;
	double GetCharacterCE() const;
	double GetCharacterMaxCE() const;
	double GetCharacterPreviousCE() const;

	void UpdatePreviousCE();
    double GetPreviousCE() const;
	
	double GetDamageReinforcement()const override;
	double GetReinforcement()const;
	double GetMaxReinforcement()const;
	double GetReinforcementCostMult() const;

	void SetMaxReinforcement(double);
	void SetCurrentReinforcement(double);
	void SetReinforcementCostMult(double);
	void AddReinforcement(double);
	void TickReinforcement();

	void Attack(Character*) override;

	int GetDomainLimit() const;
    bool DomainActive() const;
    void ActivateDomain();
    void DeactivateDomain();
    void TickDomain();
    void DomainDrain();

    bool CounterDomainActive() const;
    void ActivateCounterDomain();
    void DeactivateCounterDomain();

    bool DomainAmplificationActive() const;
    void SetAmplification(bool);

    void TickZone();
    void RecoverBurnout();
    void RecoverTechniqueBurnout(Technique*);

    Shikigami* ChooseShikigami(size_t) const;
    const std::vector<std::unique_ptr<Shikigami>>& GetShikigami() const;
    void TickShikigami(Battlefield& bf);

	void SetTechnique(std::unique_ptr<Technique>);
	void SetDomain(std::unique_ptr<Domain>);
	void SetCounterDomain(std::unique_ptr<Domain>);
	void SetSpecial(std::unique_ptr<Specials>);
	void AddShikigami(std::unique_ptr<Shikigami>);
	void AddBindingVow(std::unique_ptr<BindingVow>);
	void RemoveBindingVow(size_t);
	
	int GetBlackFlashChance() const;
	double GetBlackflashMult() const;
	void SetBlackflashChance(int);

	const std::vector<std::unique_ptr<BindingVow>>& GetBindingVows() const;
	void TickBindingVows();

	bool IsaCurseUser() const override;
	bool CanBeHit() const override;

	bool IsStrained() const;

	void SetDomainLimit(int);
	void SetMaxZoneTime(int);
	void SetMaxDomainTime(int);
	void SetBlackFlashMult(double);
	void SetMaxBurnoutTime(int);
};