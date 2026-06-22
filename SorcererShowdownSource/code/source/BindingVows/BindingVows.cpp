#include "code/header/BindingVows/BindingVows.h"
#include "code/header/BindingVows/BindingVowList.h"
#include "code/header/Characters/CurseUsers/CurseUser.h"
#include "code/header/GameManagement/Colors.h"

BindingVow::~BindingVow() = default;

void BindingVow::TickVow(CurseUser* user){
    if (!saved) SaveCharacterData(user);
    UseBindingVow();
}

std::string BindingVow::GetVowDetails() const {
    return std::format("{}{}{}\n- {}",color,name,Color::Clear, description);
}

void BindingVow::SetForRemoval(bool b){
    set_for_removal = b;
}
bool BindingVow::NeedsRemoval() const{
    return set_for_removal;
}

const std::vector<std::unique_ptr<BindingVow>>& BindingVow::GetBindingVows() {
    static std::vector<std::unique_ptr<BindingVow>> vows;
    if (vows.empty()) {
        vows.push_back(std::make_unique<BrittleEfficiency>());
        vows.push_back(std::make_unique<CursedEnergySacrifice>());
        vows.push_back(std::make_unique<BareHanded>());
    }
    return vows;
}