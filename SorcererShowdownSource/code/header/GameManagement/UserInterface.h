#pragma once

class Sorcerer;
class Character;

namespace UserInterface{
	std::string StringPad(std::string_view, size_t);
	void ShowBattleEntry(const std::vector<std::unique_ptr<Character>>& battlefield);
	void DisplaySorcererStatus(Character* s);
	void GetPlayerActions(Character* s);
	void ContinuePrompt(bool);
	void ClearScreen();
};