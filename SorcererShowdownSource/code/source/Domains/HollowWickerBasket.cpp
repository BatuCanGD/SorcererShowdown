#include "HollowWickerBasket.h"
#include "Character.h"



HollowWickerBasket::HollowWickerBasket() : Domain(150.0, 2, 2) {
    is_neutralizer = true;
    domain_name = "Hollow Wicker Basket";
    domain_color = "\033[95m";
    domain_cost = 100.0;
}
std::unique_ptr<Domain> HollowWickerBasket::Clone() const {
    return std::make_unique<HollowWickerBasket>(*this);
}