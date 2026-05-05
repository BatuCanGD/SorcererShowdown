#pragma once
#include "std.h"

class Sorcerer;
class Character;

class UserInterface {
public:
	void ShowBattleEntry(const std::vector<std::unique_ptr<Character>>& battlefield);
	void DisplaySorcererStatus(Character* s);
	static void ContinuePrompt();
	static void ClearScreen();
};