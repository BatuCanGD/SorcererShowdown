#pragma once
class Character;
class CurseUser;
class Sorcerer;
struct Battlefield;

struct TargetNeeds{
    Character* target{};
    bool needs_amp{};
};

struct CharacterBrain {
public:
    virtual ~CharacterBrain() = default;
    void ExecuteTurn(Character* user, Battlefield& bf);
    virtual std::unique_ptr<CharacterBrain> Clone() const = 0;
protected:
    TargetNeeds t_rex;
    void AttackTarget(Character* user);
    virtual void GetTarget(Character* user, Battlefield& bf) = 0;
    virtual void UseRCT(Sorcerer* user) = 0;
    virtual void UseReinforcement(CurseUser* user) = 0;
    virtual void UseShikigami(CurseUser* user) = 0;
    virtual bool TryDomainActions(CurseUser* user, Battlefield& bf) = 0;
    virtual bool TryTechniqueActions(CurseUser* user, Battlefield& bf) = 0;
    virtual bool TryInventoryActions(Character* user) = 0;
};