#include "code/header/Characters/Shikigami/Shikigami.h"
#include "code/header/Characters/CurseUsers/Sorcerers/Sorcerer.h"



Shikigami::Shikigami(double hp) : Character(hp) {}

bool Shikigami::CanBeAssignedID() const {
    return false;
}

void Shikigami::PartiallyManifest() {
    shikigami_state = State::Partial;
}

void Shikigami::Manifest() {
    shikigami_state = State::Full;
}

void Shikigami::Withdraw() {
    shikigami_state = State::Shadow;
}

void Shikigami::ActiveTimeIncrementor() {
    if (!IsActive()) return;
    active_turn_amount++;
}

bool Shikigami::IsActive() const {
    return shikigami_state == State::Partial ||
            shikigami_state == State::Full;
}

int Shikigami::GetActiveTime() const {
    return active_turn_amount;
}

bool Shikigami::IsPartiallyActive() const {
    return shikigami_state == State::Partial;
}

bool Shikigami::IsActivePhysically() const {
    return shikigami_state == State::Full;
}

std::string Shikigami::GetShikigamiStatus() const {
    switch (shikigami_state) {
    case State::Shadow: return "Dormant";
    case State::Partial: return "Ability Active";
    case State::Full: return "Physically Manifested";
    default: return "Dormant";
    }
}

bool Shikigami::IsShikigami() const {
    return true;
}

bool Shikigami::CanBeHit() const {
    if (IsActivePhysically()) {
        return true;
    }
    return false;
}

bool Shikigami::IsMahoraga() const {
    return false;
}
bool Shikigami::IsAgito() const {
    return false;
}