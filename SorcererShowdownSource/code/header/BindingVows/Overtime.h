#pragma once
#include "code/header/BindingVows/BindingVows.h"

class CurseUser;

class Overtime : public BindingVow {
protected:
    double saved_reinforcement_cost;
    double saved_max_reinforcement;
public:
    Overtime();
    void SaveCharacterData(CurseUser*) override;
    void UseBindingVow() override;
    void TickVow(CurseUser*) override;
};