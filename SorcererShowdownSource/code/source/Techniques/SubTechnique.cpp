#include "code/header/Techniques/SubTechniques.h"
#include "code/header/Characters/CurseUsers/CurseUser.h"

SubTechnique::~SubTechnique() = default;

bool SubTechnique::UsedMoreThanAmount() const{
    return true;
}
bool SubTechnique::CanBeUsed() const {
    return true;
}
double SubTechnique::CalculateDamage(CurseUser* user, double dmg) const{
    double multiplier = user->GetTechnique()->GetTechniqueOutput();
    double currentCE = user->GetCharacterCE();
    if (currentCE < dmg) {
        std::println("Insufficient Cursed Energy! Output weakened.");
        user->SpendCE(currentCE); 
        return currentCE * multiplier;
    }
    user->SpendCE(dmg);
    return dmg * multiplier;
}