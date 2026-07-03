#include "code/header/Techniques/Limitless/Limitless.h"
#include "code/header/GameManagement/BattlefieldHeader.h"
#include "code/header/Characters/CurseUsers/Sorcerers/Sorcerer.h"
#include "code/header/Techniques/Limitless/LimitlessTechnique.h"
#include "code/header/Techniques/Techniques.h"
#include "code/header/Specials/Specials.h"
#include "code/header/Specials/UnlimitedPurple.h"
#include "code/header/GameManagement/Utils.h"
#include "code/header/GameManagement/Colors.h"



UnlimitedPurple::UnlimitedPurple() {
	name = "Unlimited Purple";
	color = "\033[35m";
}

bool UnlimitedPurple::CheckSpecial(CurseUser* user) {
	if (!user->GetTechnique() || !user->GetTechnique()->IsLimitless()) return false; 
	auto lim = static_cast<Limitless*>(user->GetTechnique()); bool player = user->IsThePlayer();
	
	if (!lim->GetBlue()->UsedMoreThanAmount() || !lim->GetRed()->UsedMoreThanAmount() || !lim->GetPurple()->UsedMoreThanAmount()){
		if (player) std::println("\n{}Unlimited Hollow Purple is not ready yet! Keep using the Limitless{}",Color::DimGray,Color::Clear);
		return false;
	}
	if (!lim->FullyChanted()){
		if (player) std::println("\n{}Unlimited Hollow Purple fails to manifest! You need to Increase its output!{}",Color::DimGray,Color::Clear);
		return false;
	}
	return true;
}

void UnlimitedPurple::UseSpecial(CurseUser* user, Character*, Battlefield& bf) {
	if (!CheckSpecial(user)) return;

	if (user->IsThePlayer()){
		std::println("You have unlocked Unlimited Hollow Purple! Continue?\n 1 - Yes | 2 - No\n=> ");
		int choice = Utilities::GetInput<int>();
		if (choice != 1) return;
	}
	std::println(
		"{0}              ===============              \n"
		"{0}         =========================         \n"
		"{0}     =================================     \n"
		"{0}  =======================================  \n"
		"{0}===========UNLIMITED==HOLLOW==PURPLE===========\n"
		"{0}  =======================================  \n"
		"{0}     =================================     \n"
		"{0}         =========================         \n"
		"{0}              ===============              {1}",
		Color::Purple, Color::Clear
	);
	for(const auto& s : bf.battlefield){
		if (s.get() == user) { s->Damage(damage * 0.15); }
		else { s->Damage(damage); }
	}
}


std::unique_ptr<Specials> UnlimitedPurple::Clone() const {
	return std::make_unique<UnlimitedPurple>(*this);
}

bool UnlimitedPurple::IsUnlimitedPurple() const {
	return true;
}
