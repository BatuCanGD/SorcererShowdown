#include "Characters/Shikigami/Shikigami.h"
#include "Characters/CurseUsers/CurseUser.h"
#include "GameManagement/Colors.h"

Shikigami::~Shikigami() = default;
Shikigami::Shikigami(std::string n, std::string c) : name(n), color(c) {}

std::string Shikigami::GetShikigamiStatus() const {
    switch (shikigami_state) {
    case State::Disabled: return "Dormant";
    case State::Active: return "Active";
    default: return "Dormant";
    }
}

std::string Shikigami::GetName() const { return std::format("{}{}{}", color, name, Color::Clear); }

void Shikigami::Manifest() { shikigami_state = State::Active; }
void Shikigami::Withdraw() { shikigami_state = State::Disabled; }
void Shikigami::IncrementActiveTime() { if (!IsActive()) return; active_turn_amount++; }

int Shikigami::GetActiveTime() const { return active_turn_amount; }

bool Shikigami::IsActive() const { return shikigami_state == State::Active; }
bool Shikigami::IsMahoraga() const { return false; }
bool Shikigami::IsAgito() const { return false; }
