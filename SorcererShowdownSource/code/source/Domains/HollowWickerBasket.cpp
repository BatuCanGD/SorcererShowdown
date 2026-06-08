#include "code/header/Domains/HollowWickerBasket.h"
#include "code/header/Characters/Character.h"



HollowWickerBasket::HollowWickerBasket() : Domain(1.0, 1.0, 1) {
    is_neutralizer = true;
    name = "Hollow Wicker Basket";
    color = "\033[95m";
    domain_cost = 100.0;
}
std::unique_ptr<Domain> HollowWickerBasket::Clone() const {
    return std::make_unique<HollowWickerBasket>(*this);
}