#pragma once
#include "json.hpp"
#include "code/header/Characters/Character.h"

using json = nlohmann::json;
struct Battlefield;
class Technique;
class Domain;
class Specials;
class CursedTool;
class Shikigami;

class CharacterCreator {
public:
	static	std::unique_ptr<Character> CreateFromJson(const json&);
	void LoadJsonCharacter(Battlefield&);
};