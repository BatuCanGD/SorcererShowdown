#pragma once
#include "code/header/GameManagement/BattlefieldHeader.h"
#include "code/header/GameManagement/Utils.h"
#include "code/header/Characters/Character.h"
#include "code/header/Characters/CurseUsers/CurseUser.h"
#include "code/header/Domains/Domain.h"
#include "code/header/Techniques/Techniques.h"

namespace VList {
    inline Character* TargetSelector(Battlefield& bf) {
    	std::println("Choose your target:");
    	for (size_t i = 0; i < bf.battlefield.size(); ++i) {
    		auto& current = *bf.battlefield[i];
    		double health = current.GetCharacterHealth();
    		double cursed_energy = 0;
    		Technique* tech = nullptr;
    		Domain* domain = nullptr;

    		std::string t_status = "";
    		std::string d_status = "";

    		if (current.IsaCurseUser()){
    			auto cr = static_cast<CurseUser*>(&current);
    			tech = cr->GetTechnique();
    			domain = cr->GetDomain();
    			cursed_energy = cr->GetCharacterCE();
    			t_status = (tech == nullptr) ? "" : std::format("| Technique status: [{}] ", tech->GetStringStatus());
    			d_status = (domain == nullptr) ? "" : std::format("| Domain status: [{}] ", cr->GetDomainStatus());
    		}
    		std::string stunned = current.IsCharacterStunned() ? " (Stunned)" : "";
    		std::string name = current.GetName();
    		std::string ce_display = current.IsPhysicallyGifted() ? "Heavenly Restricted" : std::format("{:.1f} CE", cursed_energy);
    		if (bf.battlefield[i].get()->IsThePlayer()) {
    			std::println("{}: {} (You)",
    							i, name);
    			continue;
    		}
    		std::println("{}: {}{} | ({:.1f} HP) ({}) {}{} ",
    					i, name, stunned, health, ce_display, t_status, d_status);
    	}

    	std::print("=> ");
    	size_t t = Utilities::GetInput<size_t>();
        while(t == 0 || t >= bf.battlefield.size() || bf.battlefield[t]->GetCharacterHealth() <= 0.0){
            std::println("Target invalid!\n=> ");
            t = Utilities::GetInput<size_t>();
        }
    	return bf.battlefield[t].get();
    }
}
