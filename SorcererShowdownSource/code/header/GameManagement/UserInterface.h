#pragma once

class Sorcerer;
class Character;

class UserInterface {
public:
	void ShowBattleEntry(const std::vector<std::unique_ptr<Character>>& battlefield);
	void DisplaySorcererStatus(Character* s);
	void GetPlayerActions(Character* s);
	static void ContinuePrompt(bool);
	static void ClearScreen();
};