#pragma once

class Player
{
	int mX = 0;
	int mY = 0;
	int Width = 5;
	int Height = 5;
	char mCharacter = '-';
	int HP = 20;
	int team = 0;
	
public:
	enum Type { Archer, Warrior, Sorcerer };
	Type mmType;
	Player();
	int GetTeam() { return team; }
	int GetHP() { return HP; }
	Type GetType(){ return mmType; }
	void SetHP(int _lives) { HP = _lives; }
	Player(int x, int y, char Player, Type mType, int Team);
	void Update(bool mCanMove);
	void Render() const;
	int GetmX() const { return mX; }
	int GetmY() const { return mY; }
	char GetCharacter() const { return mCharacter; }
	void SetmX(int x) { mX = x; }
	void SetmY(int y) { mY = y; }
	void SetCharacter(char Player) { mCharacter = Player; }
	~Player();

};

