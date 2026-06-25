#pragma once

class Character;
class CurseUser;
class Sorcerer;

struct PlayerType{
	friend class PlayerManager;
	Character* player = nullptr;
	CurseUser* crs = nullptr;
	Sorcerer* src = nullptr;

	PlayerType(Character* pl) : player(pl) {}
	PlayerType() = default;
private:
	void FindPlayerType();
};
