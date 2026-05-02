#include "SimpleDomain.h"
#include "Character.h"



SimpleDomain::SimpleDomain() : Domain(INT32_MAX, 3, 3) {
    is_neutralizer = true;
    domain_name = "Simple Domain";
    domain_color = "\033[36m";
    domain_cost = 150.0;
}
std::unique_ptr<Domain> SimpleDomain::Clone() const {
    return std::make_unique<SimpleDomain>(*this);
}