#pragma once
#include "code/header/Techniques/Techniques.h"
#include "code/header/Techniques/SubTechniques.h"
#include "code/header/Techniques/Limitless/Blue.h"
#include "code/header/Techniques/Limitless/Red.h"
#include "code/header/Techniques/Limitless/Purple.h"

class CurseUser;
class Character;
struct Battlefield;

class Limitless : public Technique { 
protected:
	bool Infinity = true;
	std::unique_ptr<Red> red = nullptr;
    std::unique_ptr<Blue> blue = nullptr;
    std::unique_ptr<Purple> purple = nullptr;
public:
	Limitless();
	Limitless(const Limitless&);
	std::unique_ptr<Technique> Clone() const override;

	void SetInfinity(bool s);
	void InfinityNerf(CurseUser*);
	bool CheckInfinity() const;

	Blue* GetBlue() const;
	Red* GetRed() const;
	Purple* GetPurple() const;

	void TechniqueMenu(CurseUser* user, Character* target, Battlefield&) override;
	bool AutoTechniqueUse(CurseUser* user, Character* target, Battlefield& bf) override;
	void TechniqueSetting(CurseUser*, Battlefield&) override;
	void Chant() override;

	bool IsLimitless() const override;
	bool IsInfinityActive() const override;
};
