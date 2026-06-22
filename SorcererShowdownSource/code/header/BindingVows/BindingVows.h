#pragma once

class CurseUser;

class BindingVow {
protected:
    CurseUser* parent = nullptr;
    std::string name = "", color = "", description = "";
    bool saved = false, applied = false, set_for_removal = false;
public:
    virtual ~BindingVow();
    virtual std::unique_ptr<BindingVow> Clone() const = 0;
    
    void TickVow(CurseUser*);
    virtual void SaveCharacterData(CurseUser*) = 0;
    virtual void RemoveEffects() = 0;
    virtual void UseBindingVow() = 0;
    
    void SetForRemoval(bool);

    std::string GetVowDetails() const;
    static const std::vector<std::unique_ptr<BindingVow>>& GetBindingVows();
};
