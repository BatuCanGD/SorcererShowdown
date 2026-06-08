#pragma once
#include "code/header/BindingVows/BindingVows.h"

class CurseUser;

class BareHanded : public BindingVow {
protected:
    double saved_regen;
    double saved_ce;
    double saved_damage;
public:
    BareHanded();
    std::unique_ptr<BindingVow> Clone() const override;
    void SaveCharacterData(CurseUser*) override;
    void UseBindingVow() override;
    void TickVow(CurseUser*) override;
};