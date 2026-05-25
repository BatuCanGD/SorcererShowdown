#include "code/header/Characters/CurseUsers/CursedSpirits/Mahito.h"
#include "code/header/Characters/CurseUsers/CursedSpirits/TransfiguredHuman.h"
#include "code/header/GameManagement/BattlefieldHeader.h"
#include "code/header/Techniques/IdleTransfiguration.h"
#include "code/header/Domains/SelfEmbodimentOfPerfection.h"



Mahito::Mahito() : CursedSpirit(650.0, 5500.0, 40.0) {
	technique = std::make_unique<IdleTransfiguration>();
	domain = std::make_unique<SelfEmbodimentOfPerfection>();
	passive_health_regen = 35.0;
	black_flash_chance = 20;
	base_attack_damage = 50.0;
	reinforcement_cost_mult = 0.8;

	name = "Mahito";
	color = "\033[35m";
}

std::unique_ptr<Character> Mahito::Clone() const {
	return std::make_unique<Mahito>();
}

void Mahito::OnCharacterTurn(Battlefield& bf){
	if (this->IsCharacterStunned()) {
		std::println("{} is stunned and their turn will be skipped", this->GetNameWithID());
		return;
	}
	IdleTransfiguration* tf = static_cast<IdleTransfiguration*>(this->GetTechnique());

	double weakest_hp_pr = 1.1;
	Character* weakest = nullptr;
	int tf_amount = 0;
	bool summon_humans = true;

	for (const auto& chr : bf.battlefield) {
		if (chr.get() == this) continue;
		if (chr->IsaCursedSpirit()) {
			auto cs = static_cast<CursedSpirit*>(chr.get());
			if (cs->IsTransfigured()) tf_amount++;
		}
		double character_pr = chr->GetCharacterHealth() / chr->GetCharacterMaxHealth();
		if (character_pr < weakest_hp_pr || !weakest) {
			weakest = chr.get();
			weakest_hp_pr = character_pr;
		}
	}
	if (tf_amount == static_cast<int>(bf.battlefield.size() - 1)) summon_humans = false;
	else if (tf_amount == 0 || tf->GetTFcount() > 5) summon_humans = true;
	else summon_humans = false;  

	if (summon_humans && tf->GetTFcount() > 0) {
		int summon_amount = 0;
		std::println("{} is releasing a swarm of transfigured humans!", this->GetNameWithID());
		while (tf->GetTFcount() > 0) {
			tf->SummonTransfiguredHumans(bf);
			summon_amount++;
		}
		if (summon_amount > 1) {
			std::println("{} has summoned a total of {} transfigured humans!", this->GetNameWithID(), summon_amount);
		}
		else {
			std::println("{} has summoned a transfigured human!", this->GetNameWithID());
		}
		return;
	}
	else if (this->GetDomainUses() < 5 && !this->DomainActive())
	{
		if ((!this->HPMoreThanMax(0.40) || tf->Boosted()) && !tf->BurntOut())
		{
			this->ActivateDomain();
			return;
		}
	}
	if (!tf->BurntOut() && weakest && this->CEMoreThanMax(0.03)) {
		tf->UseTransfiguration(this, weakest);
		return;
	}
	this->Attack(weakest);
}