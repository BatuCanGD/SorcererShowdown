#pragma once
#include "code/header/Domains/Domain.h"

class CurseUser;
class Character;

class SelfEmbodimentOfPerfection : public Domain {
public:
	SelfEmbodimentOfPerfection();
	std::unique_ptr<Domain> Clone() const override;
	void OnSureHit(CurseUser& user, Character& target) override;
};