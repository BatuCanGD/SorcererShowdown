#include "Characters/CurseUsers/CursedSpirits/Mahito.h"
#include "GameManagement/Utils.h"
#include "GameManagement/BattlefieldHeader.h"
#include "Techniques/IdleTransfiguration.h"
#include "Domains/SelfEmbodimentOfPerfection.h"



Mahito::Mahito() : CursedSpirit(650.0, 5500.0, 40.0) {
	technique = std::make_unique<IdleTransfiguration>();
	domain = std::make_unique<SelfEmbodimentOfPerfection>();
	passive_health_regen = 35.0;
	black_flash_chance = 20;
	attack_damage = 50.0;
	reinforcement_cost_mult = 0.8;

	name = "Mahito";
	color = "\033[35m";
}

std::unique_ptr<Character> Mahito::Clone() const {
	return std::make_unique<Mahito>();
}

void Mahito::OnCharacterTurn(Battlefield& bf){
	if (IsCharacterStunned()) {
		std::println("{} is stunned and their turn will be skipped", GetNameWithID());
		return;
	}
	
	double weakest_hp = 1.1;
	Character* weakest = nullptr;

	int tf_amount = 0;
	for (const auto& chr : bf.battlefield) {
		if (chr.get() == this || chr->GetCharacterHealth() <= 0.0) continue;
		if (chr->IsaCursedSpirit()) {
			if (static_cast<CursedSpirit*>(chr.get())->IsTransfigured()) tf_amount++;
		}
		double character_hp = chr->GetCharacterHealth() / chr->GetCharacterMaxHealth();
		character_hp += Utilities::GetRandom(-5.0, 5.0) * 0.01;
    	if (character_hp < weakest_hp) {
        	weakest = chr.get();
        	weakest_hp = character_hp;
    	}
	}

	auto* tf = static_cast<IdleTransfiguration*>(GetTechnique());
	bool can_summon_humans = (tf_amount == 0 && tf->GetTFcount() >= Utilities::GetRandom<int>(3, 10));
	bool can_use_domain = GetDomain()->GetDomainUses() < domain_limit && !domain->IsActive() && !domain->OnCooldown();
	
	if (can_summon_humans) {
		std::println("{} is releasing a swarm of transfigured humans!", GetNameWithID());
		while (tf->GetTFcount() > 0) { tf->SummonTransfiguredHumans(bf); }
		return;
	}
	if (can_use_domain){
		if (!HPMoreThanMax(0.40) || tf->Boosted()){
			domain->SetDomainActivation(this, true);
			return;
		}
	}
	if (weakest && !tf->BurntOut() && CEMoreThanMax(0.03)) {
		tf->UseTransfiguration(this, weakest);
		return;
	}
	Attack(weakest);
}