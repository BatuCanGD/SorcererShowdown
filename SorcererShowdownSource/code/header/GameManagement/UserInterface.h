#pragma once

class Sorcerer;
class Character;

namespace UserInterface{
	void ShowBattleEntry(const std::vector<std::unique_ptr<Character>>& battlefield);
	void DisplaySorcererStatus(Character* s);
	void ContinuePrompt(bool);
	void ClearScreen();
};