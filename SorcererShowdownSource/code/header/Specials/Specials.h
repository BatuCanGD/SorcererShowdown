#pragma once
class CurseUser;

class Specials {
protected:
	std::string name = "";
	std::string color = "";
public:
	virtual ~Specials();
	virtual std::unique_ptr<Specials> Clone() const = 0;
	virtual void PerformSpecial(CurseUser* user) = 0;
	virtual std::string GetSpecialName() const;

	virtual bool IsUnlimitedPurple() const;
	virtual bool IsWorldCuttingSlash() const;
};