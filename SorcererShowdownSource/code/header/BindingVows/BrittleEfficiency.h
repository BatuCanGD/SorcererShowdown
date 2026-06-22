#pragma once
#include "code/header/BindingVows/BindingVows.h"

class CurseUser;

class BrittleEfficiency : public BindingVow {
protected:
    double saved_reinforcement_cost;
    double saved_max_reinforcement;
public:
    BrittleEfficiency();
    std::unique_ptr<BindingVow> Clone() const override;
    void SaveCharacterData(CurseUser*) override;
    void UseBindingVow() override;
    void RemoveEffects() override;
};