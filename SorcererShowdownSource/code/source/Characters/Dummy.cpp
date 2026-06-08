#include "code/header/Characters/Dummy.h"

Dummy::Dummy() : Character(100000.0) {
    name = "Training Dummy";
}
void Dummy::OnCharacterTurn(Battlefield&) {
    std::println("Turn #{}", ++i);
}
std::unique_ptr<Character> Dummy::Clone() const {
    return std::make_unique<Dummy>();
}