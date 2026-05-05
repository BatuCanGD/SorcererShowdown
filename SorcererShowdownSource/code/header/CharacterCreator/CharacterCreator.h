#pragma once
#include "json.hpp"
#include "Character.h"

using json = nlohmann::json;
struct Battlefield;
class Technique;
class Domain;
class Specials;
class CursedTool;
class Shikigami;

class CharacterCreator {
public:
	static std::map<Character*, json> registry;
	static	std::unique_ptr<Character> CreateFromJson(const json&);
	void LoadJsonCharacter(Battlefield&);
};