#pragma once
#include "code/header/Characters/Shikigami/Shikigami.h"

class CurseUser;
struct Battlefield;

class Mahoraga : public Shikigami {
protected:
	const double keep_active_cost = 120.0;
	enum class InfinityAdaptation {
		None,
		FirstSpin,
		SecondSpin,
		ThirdSpin,
		FourthSpin
	};
	InfinityAdaptation InfStage = InfinityAdaptation::None;
	InfinityAdaptation PrevState = InfStage;
public:
	Mahoraga();
	void Adapt();
	bool FullyAdapted()const;
	void OnShikigamiTurn(CurseUser* user, Battlefield& bf) override;
	void PrintStatus(CurseUser*) const;
	void UpdatePreviousState();
	bool IsMahoraga() const override;
};