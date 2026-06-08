#include "code/header/Techniques/Limitless/Limitless.h"
#include "code/header/Characters/CurseUsers/Sorcerers/Sorcerer.h"
#include "code/header/Techniques/Limitless/Blue.h"
#include "code/header/Techniques/Limitless/Red.h"
#include "code/header/Techniques/Limitless/Purple.h"
#include "code/header/Techniques/Limitless/UnlimitedHollowPurple.h"
#include "code/header/Techniques/Techniques.h"
#include "code/header/Specials/Specials.h"
#include "code/header/Specials/UnlimitedPurple.h"
#include "code/header/GameManagement/Utils.h"



UnlimitedPurple::UnlimitedPurple() {
	name = "Unlimited Purple";
	color = "\033[35m";
}

void UnlimitedPurple::PerformSpecial(CurseUser* user) {
	if (!user->GetTechnique() || !user->GetTechnique()->IsLimitless()) return; 

	auto limitless = static_cast<Limitless*>(user->GetTechnique());
	if (limitless->GetPurple()->GetUnlimitedHollowPurple()->CanBeUsed()) return;

	if (limitless->GetBlue()->UsedMoreThanAmount() && 
		limitless->GetRed()->UsedMoreThanAmount() && 
		limitless->GetPurple()->UsedMoreThanAmount()) 
	{
		if (!limitless->GetPurple()->GetUnlimitedHollowPurple()->CanBeUsed()) {
			std::println("\n{}=== UNLIMITED HOLLOW PURPLE IS NOW POSSIBLE! ==={}",Utilities::Color::Purple,Utilities::Color::Clear);
			limitless->GetPurple()->GetUnlimitedHollowPurple()->SetAllowance(true);
		}
	}
	else {
		std::println("\n{}Unlimited Hollow Purple fails to manifest, the limitless needs to be used more!{}",Utilities::Color::DimGray,Utilities::Color::Clear);
	}
	
}

std::unique_ptr<Specials> UnlimitedPurple::Clone() const {
	return std::make_unique<UnlimitedPurple>(*this);
}

bool UnlimitedPurple::IsUnlimitedPurple() const {
	return true;
}
