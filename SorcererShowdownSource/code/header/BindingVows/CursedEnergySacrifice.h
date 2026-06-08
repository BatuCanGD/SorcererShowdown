#pragma once
#include "code/header/BindingVows/BindingVows.h"

class CurseUser;

class CursedEnergySacrifice : public BindingVow {
protected:
    double saved_regen;
    double saved_ce;
public:
    CursedEnergySacrifice();
    std::unique_ptr<BindingVow> Clone() const override;
    void SaveCharacterData(CurseUser*) override;
    void UseBindingVow() override;
    void TickVow(CurseUser*) override;
};