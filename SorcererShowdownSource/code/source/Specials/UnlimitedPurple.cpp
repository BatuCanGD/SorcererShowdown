#include "GameManagement/BattlefieldHeader.h"
#include "Characters/CurseUsers/Sorcerers/Sorcerer.h"
#include "Techniques/Limitless.h"
#include "Specials/UnlimitedPurple.h"
#include "GameManagement/Utils.h"
#include "GameManagement/Colors.h"



UnlimitedPurple::UnlimitedPurple() {
	name = "Unlimited Purple";
	color = "\033[35m";
}

bool UnlimitedPurple::CheckSpecial(CurseUser* user) {
	auto* tech = user->GetTechnique();
	bool has_limitless = tech && tech->IsLimitless();
	bool player = user->IsThePlayer();

	if (!has_limitless){
		if (player) std::println("You don't have the necessary technique to pull this off");
		return false;
	} 
	auto* l = static_cast<Limitless*>(tech);
	if (!l->CanUseUnlimitedHollow()){
		if (player) std::println("\nUnlimited Hollow Purple is not ready yet! Keep using the Limitless");
		return false;
	}
	if (!l->FullyChanted()){
		if (player) std::println("\nUnlimited Hollow Purple fails to manifest! You need to Increase its output!");
		return false;
	}
	return true;
}

bool UnlimitedPurple::UseSpecial(CurseUser* user, Character*, Battlefield& bf) {
	if (!CheckSpecial(user)) return false;

	if (user->IsThePlayer()){
		std::println("1 - Use Unlimited Hollow Purple | 2 - Return\n=> ");
		int choice = Utilities::GetInput<int>();
		if (choice != 1) return false;
	}
	std::println(
		"{0}                 =========                 \n"
		"{0}             =================             \n"
		"{0}        ===========================        \n"
		"{0}     =================================     \n"
		"{0}  =======================================  \n"
		"{0}===========================================\n"
		"{0}===========UNLIMITED==HOLLOW==PURPLE=======\n"
		"{0}===========================================\n"
		"{0}  =======================================  \n"
		"{0}     =================================     \n"
		"{0}         =========================         \n"
		"{0}              ===============              \n"
		"{0}                 =========                {1}",
		Color::Purple, Color::Clear
	);
	for(const auto& s : bf.battlefield){
		if (s.get() == user) { s->DamageBypass(uhp_damage * 0.15); }
		else { s->DamageBypass(uhp_damage); }
	}
	return true;
}


std::unique_ptr<Specials> UnlimitedPurple::Clone() const {
	return std::make_unique<UnlimitedPurple>(*this);
}

bool UnlimitedPurple::IsUnlimitedPurple() const {
	return true;
}
