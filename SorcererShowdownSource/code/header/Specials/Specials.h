#pragma once
class CurseUser;

class Specials {
protected:
	std::string special_name = "";
	std::string special_color = "";
public:
	virtual std::unique_ptr<Specials> Clone() const = 0;
	virtual ~Specials() = default;
	virtual void PerformSpecial(CurseUser* user) = 0;
	virtual std::string GetSpecialName() const;

	virtual bool IsUnlimitedPurple() const;
	virtual bool IsWorldCuttingSlash() const;
};