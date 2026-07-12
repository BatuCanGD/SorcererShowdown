#pragma once
#include "code/header/GameManagement/BattlefieldHeader.h"
#include "code/header/GameManagement/Utils.h"
#include "code/header/Characters/CurseUsers/Sorcerers/Sorcerer.h"
#include "code/header/Domains/Domain.h"
#include "code/header/Techniques/Techniques.h"

namespace VList {
	inline Character* TargetChooser(Character* user, const Battlefield& bf) { // specifically made for quick AI random target selection
		const size_t size = bf.battlefield.size();
		if (size <= 0) return nullptr;

		Character* target = nullptr;

		do {
			target = bf.battlefield[Utilities::GetRandom<size_t>(0, size - 1)].get();
		}while(!target->IsAlive() || target == user);

		return target;
	}
    inline Character* TargetSelector(const Battlefield& bf, CurseUser* viewer) {
		bool detailed_info = viewer && viewer->IsaSorcerer() && static_cast<Sorcerer*>(viewer)->HasSixEyes();
    	std::println("Choose your target:");
    	for (size_t i = 0; i < bf.battlefield.size(); ++i) {
    		Character* current = bf.battlefield[i].get();
			std::string type = current->GetType();
			std::string name = current->GetName();

			if (current->IsThePlayer()) {
    			std::println("{}: {} | {} | (You)", i, name, type);
				continue;
    		}

    		double health = current->GetCharacterHealth();
			std::string stunned = current->IsCharacterStunned() ? " (Stunned)" : "";

    		if (detailed_info && current->IsaCurseUser()){ 
				CurseUser* cr = static_cast<CurseUser*>(current);

				std::print("{}: {}{} | {} | ({:.1f} HP) ({:.1f} CE) ", 
								i, name, stunned, type, health, cr->GetCharacterCE());
				if (Technique* tech = cr->GetTechnique()){
					std::print("| Technique status: [{}] ", tech->GetStringStatus());
				}
    			if (Domain* domain = cr->GetDomain()){
    				std::print("| Domain status: [{}] ", domain->GetDomainStatus());
				}
				std::println();
				continue;
    		}
			std::println("{}: {}{} | {} | ({:.1f} HP)", 
				i, name, stunned, type, health);
    	}

        while(true){
        	std::print("=> ");
    		size_t t = Utilities::GetInput<size_t>();
			if (t < bf.battlefield.size()){
				Character* target = bf.battlefield[t].get();
				if (!target->IsThePlayer() && target->GetCharacterHealth() > 0.0){
					return target;
				}
			}
			std::println("Target invalid!");
        }
    }
}
