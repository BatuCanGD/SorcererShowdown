#pragma once
#include "Domains/Domain.h"

class CurseUser;
class Character;

class AuthenticMutualLove final : public Domain {
public:
	AuthenticMutualLove();
	std::unique_ptr<Domain> Clone() const override;
};