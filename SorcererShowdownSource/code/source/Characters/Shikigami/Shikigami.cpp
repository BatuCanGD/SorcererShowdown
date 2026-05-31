#include "code/header/Characters/Shikigami/Shikigami.h"
#include "code/header/GameManagement/Utils.h"
#include "code/header/Characters/Character.h"
#include "code/header/Characters/CurseUsers/CurseUser.h"


Shikigami::~Shikigami() = default;
Shikigami::Shikigami() : Character(1.0) {}

std::string Shikigami::GetShikigamiStatus() const {
    switch (shikigami_state) {
    case State::Shadow: return "Dormant";
    case State::Partial: return "Ability Active";
    case State::Full: return "Physically Manifested";
    default: return "Dormant";
    }
}

std::string Shikigami::GetType() const{ return std::format("{}Shikigami{}", Utilities::Color::DimGray, Utilities::Color::Clear); }

void Shikigami::PartiallyManifest() { shikigami_state = State::Partial; }
void Shikigami::Manifest() { shikigami_state = State::Full; }
void Shikigami::Withdraw() { shikigami_state = State::Shadow; }
void Shikigami::ActiveTimeIncrementor() { if (!IsActive()) return; active_turn_amount++; }

int Shikigami::GetActiveTime() const { return active_turn_amount; }

bool Shikigami::IsActive() const { return shikigami_state == State::Partial || shikigami_state == State::Full; }
bool Shikigami::IsPartiallyActive() const { return shikigami_state == State::Partial; }
bool Shikigami::IsActivePhysically() const { return shikigami_state == State::Full; }
bool Shikigami::CanBeHit() const { return shikigami_state == State::Full; }

bool Shikigami::CanBeAssignedID() const { return false; }
bool Shikigami::IsShikigami() const { return true; }
bool Shikigami::IsMahoraga() const { return false; }
bool Shikigami::IsAgito() const { return false; }
