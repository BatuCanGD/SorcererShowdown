#pragma once

class CurseUser;

class BindingVow {
protected:
    CurseUser* parent = nullptr;
    std::string name = ""; std::string description = "";
    bool saved = false; bool applied = false; bool set_for_removal = false;
public:
    virtual ~BindingVow();
    virtual std::unique_ptr<BindingVow> Clone() const = 0;
    
    virtual void SaveCharacterData(CurseUser*) = 0;
    virtual void UseBindingVow() = 0;
    virtual void TickVow(CurseUser*);
    
    void SetForRemoval(bool);
    bool NeedsRemoval() const;

    std::string GetVowDetails() const;
    static const std::vector<std::unique_ptr<BindingVow>>& GetBindingVows();
};
