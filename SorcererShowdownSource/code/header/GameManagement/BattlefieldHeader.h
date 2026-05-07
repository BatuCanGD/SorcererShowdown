#pragma once
class Character;

struct Battlefield {
	std::vector<std::unique_ptr<Character>> battlefield;
	std::vector<std::unique_ptr<Character>> spawn_queue;
	std::vector<std::unique_ptr<Character>> characterlist;
	std::map<std::string, int> fighter_counts;
};