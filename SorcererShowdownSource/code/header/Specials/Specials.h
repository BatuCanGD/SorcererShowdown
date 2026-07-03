#pragma once
class CurseUser;
class Character;
struct Battlefield;

class Specials {
protected:
	std::string name = "";
	std::string color = "";
public:
	virtual ~Specials();
	virtual std::unique_ptr<Specials> Clone() const = 0;
	virtual bool CheckSpecial(CurseUser* user) = 0; // safety checks
	virtual void UseSpecial(CurseUser* user, Character* target, Battlefield& bf) = 0; // actual usage
	virtual std::string GetSpecialName() const;

	virtual bool IsUnlimitedPurple() const;
	virtual bool IsWorldCuttingSlash() const;
};