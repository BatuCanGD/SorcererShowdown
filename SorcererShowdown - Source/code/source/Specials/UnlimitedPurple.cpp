#include "Limitless.h"
#include "Sorcerer.h"
#include "Techniques.h"
#include "Specials.h"
#include "UnlimitedPurple.h"
#include "Utils.h"



UnlimitedPurple::UnlimitedPurple() {
	special_name = "Unlimited Purple";
	special_color = "\033[35m";
}

void UnlimitedPurple::PerformSpecial(CurseUser* user) {
	if (user->GetTechnique()->IsLimitless()) {
		auto limitless = static_cast<Limitless*>(user->GetTechnique());
		if (limitless->GetUsedBlueAmount() >= 15 && limitless->GetUsedRedAmount() >= 15 && limitless->GetUsedPurpleAmount() >= 5) {
			if (!limitless->UnlimitedHollowAllowed()) {
				std::println("\n{}=== UNLIMITED HOLLOW PURPLE IS NOW POSSIBLE! ==={}",Color::Purple,Color::Clear);
				limitless->SetUnlimitedHollow(true);
			}
		}
		else {
			std::println("\n{}Unlimited Hollow Purple fails to manifest, the limitless needs to be used more!{}",Color::DimGray,Color::Clear);
		}
	}
}

std::unique_ptr<Specials> UnlimitedPurple::Clone() const {
	return std::make_unique<UnlimitedPurple>(*this);
}