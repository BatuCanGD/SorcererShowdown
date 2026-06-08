#pragma once
#include "code/header/Techniques/Techniques.h"

class CurseUser;

class SubTechnique {
protected:
    int use_amount = 0;
    double damage = 1.0;
    double CalculateDamage(CurseUser* user, double) const;
public:
    virtual ~SubTechnique();
    virtual bool UsedMoreThanAmount() const;
    virtual bool CanBeUsed() const;
    virtual void UseTechnique(CurseUser*, Character*, Battlefield&, Technique::ChantLevel&) = 0;
};