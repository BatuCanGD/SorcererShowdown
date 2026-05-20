#pragma once
#include "code/header/Domains/Domain.h"
class Character;
class CurseUser;

struct Battlefield {
	std::vector<std::unique_ptr<Character>> battlefield;
	std::vector<std::unique_ptr<Character>> spawn_queue;
	std::vector<CurseUser*> active_domains;
};
struct BattleCreator{
	std::map<std::string, int> fighter_counts;
	std::vector<std::unique_ptr<Character>> characterlist;
	std::vector<std::unique_ptr<Domain>> domainlist;
};