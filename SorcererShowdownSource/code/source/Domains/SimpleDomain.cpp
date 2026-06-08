#include "code/header/Domains/SimpleDomain.h"
#include "code/header/Characters/Character.h"



SimpleDomain::SimpleDomain() : Domain(1.0, 1.0, 1) {
    is_neutralizer = true;
    name = "Simple Domain";
    color = "\033[36m";
    domain_cost = 150.0;
}
std::unique_ptr<Domain> SimpleDomain::Clone() const {
    return std::make_unique<SimpleDomain>(*this);
}