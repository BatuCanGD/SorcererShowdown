#include "code/header/Characters/Shikigami/Shikigami.h"
#include "code/header/Characters/CurseUsers/CurseUser.h"
#include "code/header/GameManagement/Colors.h"

Shikigami::~Shikigami() = default;
Shikigami::Shikigami() : Character(1.0) {}

std::string Shikigami::GetType() const {
    return std::format("{}Shikigami{}", Color::DimGray, Color::Clear);
}

std::string Shikigami::GetShikigamiStatus() const {
    switch (shikigami_state) {
    case State::Disabled: return "Dormant";
    case State::Active: return "Active";
    default: return "Dormant";
    }
}

void Shikigami::Manifest() { shikigami_state = State::Active; }
void Shikigami::Withdraw() { shikigami_state = State::Disabled; }
void Shikigami::IncrementActiveTime() { if (!IsActive()) return; active_turn_amount++; }

int Shikigami::GetActiveTime() const { return active_turn_amount; }

bool Shikigami::IsActive() const { return shikigami_state == State::Active; }
bool Shikigami::CanBeAssignedID() const { return false; }
bool Shikigami::IsShikigami() const { return true; }
bool Shikigami::IsMahoraga() const { return false; }
bool Shikigami::IsAgito() const { return false; }
