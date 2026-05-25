#pragma once

class CurseUser;

class BindingVow {
protected:
    CurseUser* parent = nullptr;
    std::string name = ""; std::string description = "";
    bool saved = false; bool applied = false;
public:
    enum class VowStatus { Active, Disabled, Barred };
    VowStatus bvs = VowStatus::Disabled;
    virtual ~BindingVow();
    virtual void SaveCharacterData(CurseUser*) = 0;
    virtual void UseBindingVow() = 0;
    virtual void TickVow(CurseUser*);
    void SetVowStatus(int);

    bool IsActive() const;
    bool IsUnused() const;
    bool IsUnavailable() const;

    std::string GetVowDetails() const;
    std::string GetVowStringStatus() const;
};
