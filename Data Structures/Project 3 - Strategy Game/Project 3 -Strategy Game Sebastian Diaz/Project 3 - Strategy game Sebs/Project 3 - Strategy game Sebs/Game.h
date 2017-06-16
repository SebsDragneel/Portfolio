#pragma once
#include "Player.h"
class Game
{

	char mCursor = 0;
	bool play;

	int NumofPlayers = 6;
	vector<Player*> Players;
	vector<Player*> OPlayers;
public:
	void Setplay(bool _play) { play = _play; }
	int GetNumofPlayers() { return NumofPlayers; }
	void SetNumofPlayers(int _NumofPlayers) { NumofPlayers = _NumofPlayers; }
	Game();
	void Update();
	void Render();
	void GameLoop();
	bool MovePhase(Player* P, int mAnswer);
	bool CanMove(Player* P);
	bool AttackPhase(Player* P, int mAnswer);
	~Game();
};
