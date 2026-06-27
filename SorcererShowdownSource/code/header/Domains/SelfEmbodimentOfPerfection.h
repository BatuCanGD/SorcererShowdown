#pragma once
#include "code/header/Domains/Domain.h"

class CurseUser;
class Character;

class SelfEmbodimentOfPerfection : public Domain {
	void DoSureHit(CurseUser& user, Character& target, bool is_blocked) override;
public:
	SelfEmbodimentOfPerfection();
	std::unique_ptr<Domain> Clone() const override;
	
};