#pragma once
#include "code/header/Characters/Character.h"
#include "code/header/Domains/Domain.h"
#include "code/header/Techniques/Techniques.h"
#include "code/header/Specials/Specials.h"
#include "code/header/Characters/Shikigami/Shikigami.h"
#include "code/header/BindingVows/BindingVows.h"

class CurseUser : public Character {
protected:
	std::unique_ptr<Domain> domain = nullptr;
	std::unique_ptr<Domain> counter_domain = nullptr;
	std::unique_ptr<Technique> technique = nullptr;
	std::unique_ptr<Specials> special = nullptr;
	std::vector<std::unique_ptr<Shikigami>> shikigami;
	std::vector<std::unique_ptr<BindingVow>> binding_vows;

	double cursed_energy;
	double max_cursed_energy;
	double prev_cursed_energy;

	double ce_regen;
	double saved_ce_regen;

	bool domain_amplification_active = false;
	bool counter_domain_active = false;
	bool domain_active = false;
	bool is_strained = false;
	bool counter_on_cooldown = false;
	bool zone_ce_boost = false;

	int total_domain_uses = 0;

	int technique_burnout_time = 0;
	int burnout_time = 0;

	int active_domain_time = 0;
	int active_counter_time = 0; 

	int counter_recover_time = 0;

	int black_flash_chance = 5;  
	int the_zone_time = 0;
	int max_zone_time = 3;

	double blackflash_mult = 4.5;
	int blackflash_chain = 0;

	double current_ce_reinforcement;
	double max_reinforcement;
	double reinforcement_cost_mult = 2.0;

	int domain_limit = 5;
	int max_counter_time = 3;
	int max_counter_cooldown = 2;
	int max_domain_time = 5;
	int max_technique_burnout_time = 4;
public:
    CurseUser(double hp, double ce, double regen);
    virtual ~CurseUser();

	Technique* GetTechnique() const;
	Specials* GetSpecial() const;
	Domain* GetCounterDomain() const;
	Domain* GetDomain() const;


	std::string GetDAstatus() const;
	std::string GetDomainStatus() const;
	std::string GetCounterStatus() const;
	std::string GetReinforcementStatus() const;

	virtual void SpendCE(double ce);
	void SpendCEdirect(double ce);
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

    bool DomainActive() const;
    void ActivateDomain();
    void DeactivateDomain();
    void TickDomain();
    void DomainDrain();
	int GetDomainUses() const;

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