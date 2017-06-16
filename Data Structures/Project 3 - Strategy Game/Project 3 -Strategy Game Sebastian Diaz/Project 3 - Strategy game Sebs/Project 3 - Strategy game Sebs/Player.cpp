#include "stdafx.h"
#include "Player.h"
#include "Game.h"

Player::Player()
{
}
Player::Player(int x, int y, char Player, Type mType, int mTeam)
{
	SetmX(x);
	SetmY(y);
	SetCharacter(Player);
	mmType = mType;
	team = mTeam;
}

void Player::Update(bool mCanMove)
{
	
}
void Player::Render() const
{
	COORD mPosition = { GetmX(), GetmY() };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), mPosition);
	cout << GetCharacter();

	
	
}

Player::~Player()
{
}
