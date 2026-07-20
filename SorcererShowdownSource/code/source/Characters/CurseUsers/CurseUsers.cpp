#include "Characters/CurseUsers/CurseUser.h"
#include "BindingVows/BindingVowList.h"
#include "CursedTools/CursedTool.h"
#include "GameManagement/BattlefieldHeader.h"
#include "Characters/Shikigami/Shikigami.h"
#include "Domains/Domain.h"
#include "Techniques/Techniques.h"
#include "Specials/Specials.h"
#include "GameManagement/Colors.h"
#include "GameManagement/Utils.h"

CurseUser::~CurseUser() = default;

CurseUser::CurseUser(double hp, double ce, double re) : 
    Character(hp),
    cursed_energy(ce),
    max_cursed_energy(ce),
    saved_max_cursed_energy(ce),
    prev_cursed_energy(ce),
    ce_regen(re),
    saved_ce_regen(re),
    current_ce_reinforcement(50.0),
    max_reinforcement(200.0){
}
std::string CurseUser::GetType() const{
    return std::format("{}Curse User{}", Color::Blue, Color::Clear);
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

void CurseUser::TickShikigami(Battlefield& bf) {
    for (const auto& s : shikigami) {
        s->OnShikigamiTurn(this, bf);
    }
}
std::string CurseUser::GetDAstatus() const {
    if (domain_amplification_active) return std::format("{}Active{}", Color::Cyan, Color::Clear);
    return std::format("{}Inactive{}", Color::Red, Color::Clear);
}
std::string CurseUser::GetReinforcementStatus() const {
    std::string currentcolor = Color::Yellow;
    std::string clear = Color::Clear;

    if (current_ce_reinforcement < GetMaxReinforcement() * 0.25) {
        currentcolor = Color::Red;
    }
    else if (current_ce_reinforcement < GetMaxReinforcement() * 0.50) {
        currentcolor = Color::Yellow;
    }
    else if (current_ce_reinforcement < GetMaxReinforcement() * 0.75) {
        currentcolor = Color::Green;
    }
    else if (current_ce_reinforcement < GetMaxReinforcement()) {
        currentcolor = Color::Blue;
    }
    else {
        currentcolor = Color::Purple;
    }
    return std::format("{}{:.1f}/{:.1f}{}", currentcolor, current_ce_reinforcement, max_reinforcement, clear);
}
void CurseUser::TickZone() {
    if ((!domain || !domain->IsActive()) && in_the_zone) {
        the_zone_time++;
        if (!zone_boosted) {
            SetCursedEnergyRegen(ce_regen * 2.5);
            zone_boosted = true;
        }
        if (the_zone_time > max_zone_time) {
            if (technique) technique->Set(Technique::Status::Usable);
            the_zone_time = 0;
        }
    }
    else {
        the_zone_time = 0;
        if (zone_boosted) {
            SetCursedEnergyRegen(saved_ce_regen);
            zone_boosted = false;
        }
    }
}

void CurseUser::Attack(Character* target) {
    if (domain_amplification_active) {
        double ce_addon = std::sqrt(std::max(0.0, GetCharacterCE())) * 0.888;
        double amp_damage = (attack_damage + ce_addon) + current_ce_reinforcement * 0.35;
        target->DamageBypass(amp_damage);
        std::println("{} landed a strike on {} using {}domain amplification{}!", GetNameWithID(), target->GetNameWithID(), Color::Yellow, Color::Clear);
        return;
    }
    if (cursed_tool) {
        cursed_tool->UseTool(this, target);
        return;
    }
    if (target->IsaCurseUser()) {
        auto target_cuser = static_cast<CurseUser*>(target);
        if (auto* tech = target_cuser->GetTechnique()) {
            if (tech->HasInvulnerabilityBarrier() && !domain_amplification_active) {
                std::println("{}'s attack was blocked by {}'s {}Barrier{}!", GetNameWithID(), target_cuser->GetNameWithID(), Color::Cyan, Color::Clear);
                return;
            }
        }
    }

    bool is_black_flash = Utilities::GetRandom(1, 100) <= black_flash_chance;
    if (is_black_flash) {
        in_the_zone = true;
        blackflash_chain++;
        if (technique) {
            technique->Set(Technique::Status::DomainBoost);
        }
        if (in_the_zone && blackflash_chain > 2){
            black_flash_chance += std::min(Utilities::GetRandom<int>(1, 3), 100);
        }
    }
    else {
        blackflash_chain = 0;
    }
    
    double final_damage = (attack_damage * (is_black_flash ? GetBlackflashMult() : 1.0)) + current_ce_reinforcement * 0.35;
    target->Damage(final_damage);
    if (is_black_flash) {
        std::println("\n******* {}BLACK FLASH!{} *******", Color::Red, Color::Clear);
        std::println("{} landed a {}BlackFlash{} on {}!", GetNameWithID(), Color::Red, Color::Clear, target->GetNameWithID());
    }
    else {
        std::println("{} landed a {}heavy strike{} on {}!", GetNameWithID(), Color::BrightRed, Color::Clear, target->GetNameWithID());
    }
}

bool CurseUser::CanBeHit() const {
    return technique ? !technique->HasInvulnerabilityBarrier() : true;
}

void CurseUser::RemoveBindingVow(size_t i) {
    if (i < binding_vows.size()) {
        binding_vows[i]->SetForRemoval(true);
        binding_vows[i]->TickVow(this);
        binding_vows.erase(binding_vows.begin() + static_cast<std::ptrdiff_t>(i));
    }
}

double CurseUser::GetEfficiencyMult() const{
    switch(ce_efficiency){
        case CEfficiency::Wasteful: return 1.65;
        case CEfficiency::Rough: return 1.35;
        case CEfficiency::Stable: return 1.0;
        case CEfficiency::Expert: return 0.70;
        case CEfficiency::Absolute: return 0.45;
        default: return 1.0;
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
void CurseUser::SetBlackFlashMult(double m){ blackflash_mult = m; }
void CurseUser::SetMaxBurnoutTime(int t){ max_technique_burnout_time = t; }

void CurseUser::RegenCE() { cursed_energy = std::min(cursed_energy + ce_regen, max_cursed_energy); }
void CurseUser::SpendCEBypass(double c) { cursed_energy = std::max(cursed_energy - c, 0.0); }
void CurseUser::SpendCE(double c) { cursed_energy = std::max(cursed_energy - (c * GetEfficiencyMult()), 0.0); }
void CurseUser::UpdatePreviousCE() { prev_cursed_energy = cursed_energy; }
void CurseUser::AddReinforcement(double r) { current_ce_reinforcement = std::clamp(current_ce_reinforcement + r, 0.0, max_reinforcement); }
void CurseUser::AddShikigami(std::unique_ptr<Shikigami> s) { shikigami.push_back(std::move(s)); }
void CurseUser::AddBindingVow(std::unique_ptr<BindingVow> vow) { binding_vows.push_back(std::move(vow)); }

bool CurseUser::CEMoreThanMax(double c) const { return GetCharacterCE() > GetCharacterMaxCE() * c; }
bool CurseUser::AmpActive() const { return domain_amplification_active; }
bool CurseUser::IsaCurseUser() const { return true; }

double CurseUser::GetBlackflashMult() const { return blackflash_mult * blackflash_chain; }
double CurseUser::GetCharacterPreviousCE() const { return prev_cursed_energy; }
double CurseUser::GetCharacterCE() const { return cursed_energy; }
double CurseUser::GetCursedEnergyRegen() const { return ce_regen; }
double CurseUser::GetCharacterMaxCE() const { return max_cursed_energy; }
double CurseUser::GetReinforcement() const { return current_ce_reinforcement; }
double CurseUser::GetMaxReinforcement()const { return max_reinforcement; }
double CurseUser::GetReinforcementCostMult() const{ return reinforcement_cost_mult; }
double CurseUser::GetPreviousCE() const { return prev_cursed_energy; }
int CurseUser::GetBlackFlashChance()const { return black_flash_chance; }
int CurseUser::GetDomainLimit() const { return domain_limit; }

Domain* CurseUser::GetCounter() const { return counter_domain.get(); }
Domain* CurseUser::GetDomain() const { return domain.get(); }
Technique* CurseUser::GetTechnique() const { return technique.get(); }
Specials* CurseUser::GetSpecial() const { return special.get(); }

const std::vector<std::unique_ptr<Shikigami>>& CurseUser::GetShikigami() const { return shikigami; }
const std::vector<std::unique_ptr<BindingVow>>& CurseUser::GetBindingVows() const { return binding_vows; }