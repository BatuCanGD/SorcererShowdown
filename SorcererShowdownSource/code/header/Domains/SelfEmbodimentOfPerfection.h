#pragma once
#include "Domains/Domain.h"

class CurseUser;
class Character;

class SelfEmbodimentOfPerfection final : public Domain {
public:
	SelfEmbodimentOfPerfection();
	std::unique_ptr<Domain> Clone() const override;
	
};