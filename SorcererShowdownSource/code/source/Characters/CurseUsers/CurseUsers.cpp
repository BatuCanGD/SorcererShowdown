#include "code/header/Characters/CurseUsers/CurseUser.h"
#include "code/header/BindingVows/BindingVowList.h"
#include "code/header/Techniques/Limitless/Limitless.h"
#include "code/header/CursedTools/CursedTool.h"
#include "code/header/GameManagement/BattlefieldHeader.h"
#include "code/header/Characters/Shikigami/Shikigami.h"
#include "code/header/Domains/Domain.h"
#include "code/header/Techniques/Techniques.h"
#include "code/header/Specials/Specials.h"
#include "code/header/GameManagement/Utils.h"

CurseUser::~CurseUser() = default;

CurseUser::CurseUser(double hp, double ce, double re) : 
    Character(hp),
    cursed_energy(ce),
    max_cursed_energy(ce),
    prev_cursed_energy(ce),
    ce_regen(re),
    saved_ce_regen(re),
    current_ce_reinforcement(50.0),
    max_reinforcement(200.0){ 
    character_type = std::format("{}Curse User{}", Utilities::Color::Blue, Utilities::Color::Clear);
}
Shikigami* CurseUser::ChooseShikigami(size_t index)  const {
    if (index < shikigami.size()) {
        return shikigami[index].get();
    }
    return nullptr;
}
void CurseUser::SetMaxCursedEnergy(double c) {
    max_cursed_energy = c;
    if (cursed_energy > max_cursed_energy){
        cursed_energy = max_cursed_energy;
    }
}
double CurseUser::GetDamageReinforcement()const {
    if (max_reinforcement <= 0.0) return 1.0;
    return 1.0 + ((current_ce_reinforcement / max_reinforcement) * 2);
}
void CurseUser::SetMaxReinforcement(double max) {
    max_reinforcement = max;
    if (current_ce_reinforcement > max_reinforcement) {
        current_ce_reinforcement = max_reinforcement;
    }
}
void CurseUser::TickBindingVows(){
    for (const auto& vow : binding_vows){
        vow->TickVow(this);
    }
}
void CurseUser::TickReinforcement() {
    if (current_ce_reinforcement <= 0.0) return;
    double maintain_cost = current_ce_reinforcement * reinforcement_cost_mult;
    SpendCE(maintain_cost);
    if (GetCharacterCE() < GetReinforcement()) {
        current_ce_reinforcement = 0.0;
        std::println("{}'s CE reinforcement collapsed due to a lack of Cursed Energy!", GetName());
    }
}
std::string CurseUser::GetDomainStatus()const {
    if (domain_active) return "\033[35mActive\033[0m";
    else return "\033[2;90mInactive\033[0m";
}
void CurseUser::TickShikigami(Battlefield& bf) {
    for (const auto& s : shikigami) {
        s->OnShikigamiTurn(this, bf);
    }
}
std::string CurseUser::GetDAstatus() const {
    if (domain_amplification_active) return std::format("{}Active{}", Utilities::Color::Cyan, Utilities::Color::Clear);
    return std::format("{}Inactive{}", Utilities::Color::Red, Utilities::Color::Clear);
}
std::string CurseUser::GetCounterStatus() const {
    if (counter_domain_active) return std::format("{}Active{}", Utilities::Color::Purple, Utilities::Color::Clear);
    return std::format("{}Inactive{}", Utilities::Color::Red, Utilities::Color::Clear);
}
std::string CurseUser::GetReinforcementStatus() const {
    std::string currentcolor = Utilities::Color::Yellow;
    std::string clear = Utilities::Color::Clear;

    if (current_ce_reinforcement < GetMaxReinforcement() * 0.25) {
        currentcolor = Utilities::Color::Red;
    }
    else if (current_ce_reinforcement < GetMaxReinforcement() * 0.50) {
        currentcolor = Utilities::Color::Yellow;
    }
    else if (current_ce_reinforcement < GetMaxReinforcement() * 0.75) {
        currentcolor = Utilities::Color::Green;
    }
    else if (current_ce_reinforcement < GetMaxReinforcement()) {
        currentcolor = Utilities::Color::Blue;
    }
    else {
        currentcolor = Utilities::Color::Purple;
    }
    return std::format("{}{:.1f}/{:.1f}{}", currentcolor, current_ce_reinforcement, max_reinforcement, clear);
}
void CurseUser::TickZone() {
    if (!technique) return;

    if (!DomainActive() && GetTechnique()->GetStatus() == Technique::Status::DomainBoost) {
        the_zone_time++;
        if (!zone_ce_boost) {
            SetCursedEnergyRegen(ce_regen + 50.0);
            zone_ce_boost = true;
        }
        if (the_zone_time > max_zone_time) {
            GetTechnique()->Set(Technique::Status::Usable);
            the_zone_time = 0;
        }
    }
    else {
        the_zone_time = 0;
        if (zone_ce_boost) {
            SetCursedEnergyRegen(saved_ce_regen);
            zone_ce_boost = false;
        }
    }
}
void CurseUser::TickDomain() {
    if (DomainActive()) {
        active_domain_time++;
        if (active_domain_time == max_domain_time) {
            std::println("{}'s domain will end soon", GetNameWithID());
        }
        else if (active_domain_time > max_domain_time) {
            std::println("{}'s domain has been deactivated after reaching its timed limit!", GetNameWithID());
            DeactivateDomain();
            GetDomain()->CollapseDomain();
            active_domain_time = 0;
        }
    }
    if (CounterDomainActive()) {
        active_counter_time++;
        if (active_counter_time == max_counter_time) {
            std::println("{}'s {}{}{} is about to shatter", GetNameWithID(), Utilities::Color::Cyan, GetCounterDomain()->GetDomainName(), Utilities::Color::Clear);
        }
        else if (active_counter_time > max_counter_time) {
            std::println("{}'s {}{}{} has been {}shattered{}!", GetNameWithID(), Utilities::Color::Cyan, GetCounterDomain()->GetDomainName(), Utilities::Color::Clear, Utilities::Color::Red, Utilities::Color::Clear);
            DeactivateCounterDomain();
            counter_on_cooldown = true;
            active_counter_time = 0;
        }
    }
    if (!CounterDomainActive()) {
        if (active_counter_time > 0) {
            counter_on_cooldown = true;
            active_counter_time = 0;
            counter_recover_time = 0; 
        }
        if (counter_on_cooldown) {
            counter_recover_time++;
            if (counter_recover_time >= max_counter_cooldown) {
                counter_on_cooldown = false;
                counter_recover_time = 0;
            }
        }
    }
}
void CurseUser::ActivateDomain() {
    if (!domain) {
        std::println("You don't have a domain to activate!");
        return;
    }
    else if (domain_active) {
        std::println("Your domain is already active!");
        return;
    }
    else if (is_strained) {
        std::println("Your brain is {}strained!{} You cannot use your domain for now...", Utilities::Color::Red, Utilities::Color::Clear);
        return;
    }
    else if (total_domain_uses >= domain_limit) {
        DamageBypass(50.0);
        SetStunState(true);
        total_domain_uses++;
        std::println("{}You have overused your domain! You take 50 damage and are stunned for the next turn.{}", Utilities::Color::Red, Utilities::Color::Clear);
        return;
    }
    domain_active = true;
    total_domain_uses++;
    std::println("\n********{}Domain Expansion{}********\n" "*******{}*******\n", Utilities::Color::Purple, Utilities::Color::Clear, GetDomain()->GetDomainName());
    if (technique) {
        technique->Set(Technique::Status::DomainBoost);
    }
}
void CurseUser::DeactivateDomain() {
    domain_active = false;
    is_strained = true;
    active_domain_time = 0;
    if (technique) {
        technique->Set(Technique::Status::BurntOut);
        technique_burnout_time = 0;
    }
}
void CurseUser::ActivateCounterDomain() {
    if (!counter_domain) {
        std::println("{} doesn't have a counter domain!", GetNameWithID());
        return;
    }
    if (counter_on_cooldown) {
        std::println("your counter domain is on cooldown, wait for it to recover!");
        return;
    }
    if (!counter_domain_active) {
        counter_domain_active = true;
        std::println("{} activates {}!", GetNameWithID(), counter_domain->GetDomainName());
    }
}
void CurseUser::DeactivateCounterDomain() {
    if (!counter_domain) {
        std::println("{} doesn't have a counter domain!", GetNameWithID());
        return;
    }
    if (counter_domain_active) {
        counter_domain_active = false;
        std::println("{} deactivated {}!", GetNameWithID(), counter_domain->GetDomainName());
    }
}
void CurseUser::Attack(Character* target) {
    if (domain_amplification_active) {
        double ce_addon = std::sqrt(std::max(0.0, GetCharacterCE())) * 0.888;
        double amp_damage = attack_damage + ce_addon;
        target->DamageBypass(amp_damage);
        std::println("{} landed a strike on {} using {}domain amplification{}!", GetNameWithID(), target->GetNameWithID(), Utilities::Color::Yellow, Utilities::Color::Clear);
        return;
    }
    if (cursed_tool) {
        cursed_tool->UseTool(this, target);
        return;
    }
    if (target->IsaCurseUser()) {
        auto target_cuser = static_cast<CurseUser*>(target);
        if (auto* tech = target_cuser->GetTechnique()) {
            if (tech->IsLimitless() && tech->IsInfinityActive() && !domain_amplification_active) {
                std::println("{}'s attack was blocked by {}'s {}Infinity{}!", GetNameWithID(), target_cuser->GetNameWithID(), Utilities::Color::Cyan, Utilities::Color::Clear);
                return;
            }
        }
    }

    bool is_black_flash = false;
    if (Utilities::GetRandomNumber(1, 100) <= black_flash_chance) {
        is_black_flash = true;
        is_strained = false;
        burnout_time = 0;
        blackflash_chain++;
        if (technique) {
            technique->Set(Technique::Status::DomainBoost);
        }
    }
    else {
        blackflash_chain = 0;
    }
    double final_damage = attack_damage * (is_black_flash ? GetBlackflashMult() : 1.0);
    target->Damage(final_damage);
    if (is_black_flash) {
        std::println("\n*** {}BLACK FLASH!{} ***", Utilities::Color::Red, Utilities::Color::Clear);
        std::println("{} landed a {}BlackFlash{} on {}!", GetNameWithID(), Utilities::Color::Red, Utilities::Color::Clear, target->GetNameWithID());
    }
    else {
        std::println("{} landed a {}heavy strike{} on {}!", GetNameWithID(), Utilities::Color::BrightRed, Utilities::Color::Clear, target->GetNameWithID());
    }
}
void CurseUser::RecoverBurnout() {
    if (is_strained) {
        burnout_time++;
        if (burnout_time >= max_technique_burnout_time) {
            is_strained = false;
            burnout_time = 0;
        }
    }
}

void CurseUser::RecoverTechniqueBurnout(Technique* t) {
    if (t == nullptr) return;
    if (t->BurntOut()) {
        technique_burnout_time++;
        if (technique_burnout_time >= max_technique_burnout_time) {
            t->Set(Technique::Status::Usable);
            technique_burnout_time = 0;
            std::println("{}'s {}cursed technique{} has{} recovered from burnout{}!", GetNameWithID(), Utilities::Color::Cyan, Utilities::Color::Clear, Utilities::Color::Green, Utilities::Color::Clear);
        }
    }
    if (technique_burnout_time != 0 && !t->BurntOut()) technique_burnout_time = 0;
}

bool CurseUser::CanBeHit() const {
    if (technique && technique->IsLimitless() && technique->IsInfinityActive()) {
        return false;
    }
    return true;
}

void CurseUser::RemoveBindingVow(size_t i) {
    if (i < binding_vows.size()) {
        binding_vows[i]->SetForRemoval(true);
        binding_vows[i]->TickVow(this);
        binding_vows.erase(binding_vows.begin() + static_cast<std::ptrdiff_t>(i));
    }
}

void CurseUser::SetAmplification(bool t) { domain_amplification_active = t; }
void CurseUser::SetCursedEnergy(double c) { cursed_energy = c; }
void CurseUser::SetCursedEnergyRegen(double c) { ce_regen = c; }
void CurseUser::SetReinforcementCostMult(double d){ reinforcement_cost_mult = d; }
void CurseUser::SetCurrentReinforcement(double r) { current_ce_reinforcement = std::clamp(r, 0.0, max_reinforcement); }
void CurseUser::SetTechnique(std::unique_ptr<Technique> t) {  technique = std::move(t); }
void CurseUser::SetDomain(std::unique_ptr<Domain> d) {  domain = std::move(d); }
void CurseUser::SetSpecial(std::unique_ptr<Specials> s) {  special = std::move(s); }
void CurseUser::SetCounterDomain(std::unique_ptr<Domain> cd) {  counter_domain = std::move(cd); }
void CurseUser::SetBlackflashChance(int d) {  black_flash_chance = d; }
void CurseUser::SetDomainLimit(int d) { domain_limit = d; }
void CurseUser::SetMaxZoneTime(int t){ max_zone_time = t; }
void CurseUser::SetMaxDomainTime(int t){ max_domain_time = t; }
void CurseUser::SetBlackFlashMult(double m){ blackflash_mult = m; }
void CurseUser::SetMaxBurnoutTime(int t){ max_technique_burnout_time = t; }

void CurseUser::RegenCE() { cursed_energy = std::min(cursed_energy + ce_regen, max_cursed_energy); }
void CurseUser::SpendCEdirect(double ce) { cursed_energy = std::max(cursed_energy - ce, 0.0); }
void CurseUser::SpendCE(double c) { cursed_energy = std::max(cursed_energy - c, 0.0); }
void CurseUser::UpdatePreviousCE() { prev_cursed_energy = cursed_energy; }
void CurseUser::AddReinforcement(double r) { current_ce_reinforcement = std::clamp(current_ce_reinforcement + r, 0.0, max_reinforcement); }
void CurseUser::DomainDrain() { SpendCE(GetDomain()->GetUseCost()); }
void CurseUser::AddShikigami(std::unique_ptr<Shikigami> s) { shikigami.push_back(std::move(s)); }
void CurseUser::AddBindingVow(std::unique_ptr<BindingVow> vow) { binding_vows.push_back(std::move(vow)); }

bool CurseUser::CEMoreThanMax(double c) const { return GetCharacterCE() > GetCharacterMaxCE() * c; }
bool CurseUser::DomainAmplificationActive() const { return domain_amplification_active; }
bool CurseUser::IsStrained() const { return is_strained; }
bool CurseUser::DomainActive() const { return domain_active; }
bool CurseUser::IsaCurseUser() const { return true; }
bool CurseUser::CounterDomainActive() const { return counter_domain_active; }

double CurseUser::GetBlackflashMult() const { return blackflash_mult * blackflash_chain; }
double CurseUser::GetCharacterPreviousCE() const { return prev_cursed_energy; }
double CurseUser::GetCharacterCE() const { return cursed_energy; }
double CurseUser::GetCursedEnergyRegen() const { return ce_regen; }
double CurseUser::GetCharacterMaxCE() const { return max_cursed_energy; }
double CurseUser::GetReinforcement() const { return current_ce_reinforcement; }
double CurseUser::GetMaxReinforcement()const { return max_reinforcement; }
double CurseUser::GetReinforcementCostMult() const{ return reinforcement_cost_mult; }

int CurseUser::GetBlackFlashChance()const { return black_flash_chance; }
int CurseUser::GetDomainUses() const { return total_domain_uses; }

Domain* CurseUser::GetCounterDomain() const { return counter_domain.get(); }
Domain* CurseUser::GetDomain() const { return domain.get(); }
Technique* CurseUser::GetTechnique() const { return technique.get(); }
Specials* CurseUser::GetSpecial() const { return special.get(); }

const std::vector<std::unique_ptr<Shikigami>>& CurseUser::GetShikigami() const { return shikigami; }
const std::vector<std::unique_ptr<BindingVow>>& CurseUser::GetBindingVows() const { return binding_vows; }