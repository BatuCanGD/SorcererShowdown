#include "code/header/BindingVows/BindingVows.h"
#include "code/header/Characters/CurseUsers/CurseUser.h"
#include "code/header/GameManagement/Utils.h"

BindingVow::~BindingVow() = default;

void BindingVow::TickVow(CurseUser* user){
    if (bvs == VowStatus::Active){
        if (!saved) SaveCharacterData(user);
        UseBindingVow();
    }
}

bool BindingVow::IsActive() const{
    return bvs == VowStatus::Active;
}
bool BindingVow::IsUnavailable() const{
    return bvs == VowStatus::Barred;
}
bool BindingVow::IsUnused() const{
    return bvs == VowStatus::Disabled;
}

void BindingVow::SetVowStatus(int ch){
    switch(ch){
    case 1:
        if (bvs != VowStatus::Active || bvs != VowStatus::Barred){
            bvs = VowStatus::Active;
        }else{
            std::println("You cannot activate this binding vow again");
        }
        break;
    case 2:
        if (bvs == VowStatus::Active){
            bvs = VowStatus::Barred;
        }else{
            std::println("You cant deactivate this binding vow");
        }
        break;
    default:
        std::println("Invalid Input");
    }
}

std::string BindingVow::GetVowDetails() const {
    return std::format("{}-{}\n- {}", name, this->GetVowStringStatus(), description);
}

std::string BindingVow::GetVowStringStatus() const{
    switch(bvs){
    case VowStatus::Active: return std::format("[{}Active{}]",Utilities::Color::Green, Utilities::Color::Clear);
    case VowStatus::Disabled: return std::format("[{}Unused{}]", Utilities::Color::DimGray, Utilities::Color::Clear);
    case VowStatus::Barred: return std::format("[{}Unusable{}] - You cant use this binding vow again", Utilities::Color::Red, Utilities::Color::Clear);
    default: return "[Fail]";
    }
}