#include "stdafx.h"
#include "Game.h"
#include "Player.h"

class DTSTimer
{
	unsigned int startTime;
public:
	DTSTimer()
	{
		reset();
	}

	void reset()
	{
		startTime = clock();
	}

	unsigned int getElapsedTime()
	{
		return (clock() - startTime);
	}

	static void delay(const unsigned int milli)
	{
		unsigned int st = clock();
		while (clock() - st < milli) {}
	}

};

Game::Game()
{
			Players.push_back(new Player(0, 0, 'A', Player::Archer, 1));
			Players.push_back(new Player(1, 0, 'W', Player::Warrior, 1));
			Players.push_back(new Player(0, 1, 'S', Player::Sorcerer, 1));
			Players.push_back(new Player(4, 5, 's', Player::Sorcerer, 2));
			Players.push_back(new Player(5, 4, 'w', Player::Warrior, 2));
			Players.push_back(new Player(5, 5, 'a', Player::Archer, 2));
			
			OPlayers = Players;
}
void Game::Update()
{
	for (int i = 0; i < NumofPlayers; ++i)
	{
		mCursor++;

		if (CanMove(Players[i]) == false)
		{
			cout << "\n\n" << Players[i]->GetCharacter() << ": Can't Move, Skipped Move Phase.. ";
			cin.get();
			system("cls");
			Render();
		}
		else if (CanMove(Players[i]) == true)
		{
			string answer;
			cout << "\n\n" << Players[i]->GetCharacter() << ": Do you want to Move? ";
			
			while (true)
			{
				if (getline(cin, answer, '\n'))
					break;
				else
				{
					cin.clear();
					cin.ignore(INT_MAX, '\n');

					cout << "You're an idiot.  No integers allowed: ";
				}
			}
			cout << '\n';
			if (answer == "yes")
			{
				unsigned int mMove;
				cout << "In wich direction you want to move? 1: Up 2: Down 3: Left 4: Right :";
				while (true)
				{
					if(!(cin >> mMove && mMove < 5 && !(mMove < 0) && MovePhase(Players[i], mMove) != false))
					{
						cin.clear();
						cin.ignore(INT_MAX, '\n');

						cout << "You're an idiot.  Can't move there... \n";
						cout << "In wich direction you want to move? 1: Up 2: Down 3: Left 4: Right :";
					}
					else break;
				}
				cout << '\n';
			}

			
			system("cls");
			Render();
		}
	}
}
void Game::Render()
{
	for (int i = 0; i < 6; i++)
	{
		cout << "******\t" << Players[i]->GetCharacter() << (mCursor == i ? "<" : " ") << '\n';
	}
	for (int i = 0; i < NumofPlayers; ++i)
	{
		OPlayers[i]->Render();
	}

	cout << "\nP1:  ";
	for (int i = 0; i < 3; ++i)
	{
		cout << OPlayers[i]->GetCharacter();
		cout << ": " << OPlayers[i]->GetHP() << "/20   ";
	}
	cout << "\nP2:  ";
	for (int i = 3; i < NumofPlayers; ++i)
	{
		cout << OPlayers[i]->GetCharacter();
		cout << ": " << OPlayers[i]->GetHP() << "/20   ";
	}

}
void Game::GameLoop()
{
	srand((unsigned int)time(NULL));
	random_shuffle(Players.begin(), Players.end());
	while (play)
	{
		DTSTimer bob;
		LockWindowUpdate(GetConsoleWindow());
		LockWindowUpdate(NULL);
		Render();
		Update();
		random_shuffle(Players.begin(), Players.end());
		mCursor = 0;
		for (int i = 0; i < NumofPlayers; i++)
		{
				if (Players[0]->GetHP() <= 0 && Players[1]->GetHP() <= 0 && Players[2]->GetHP() <= 0)
				{
					Setplay(false);
					
					cout << "Game Over\nPlayer 2 Wins";
				}
				else if (Players[3]->GetHP() <= 0 && Players[4]->GetHP() <= 0 && Players[5]->GetHP() <= 0)
				{
					Setplay(false);

					cout << "Game Over\nPlayer 1 Wins";
				}
		}

		if (GetAsyncKeyState(VK_ESCAPE))
		{
			Setplay(false);
		}

		bob.delay(100);
		system("cls");
	}
}
bool Game::CanMove(Player* P)
{
	bool mUp = true;
	bool mDown = true;
	bool mLeft = true;
	bool mRight = true;

	for (unsigned int i = 0; i < Players.size(); ++i)
	{
		if (P->GetmY() == 0)
			mUp = false;
		else if (P->GetmY() - 1 == Players[i]->GetmY() && P->GetmX() == Players[i]->GetmX())
			mUp = false;

		if(P->GetmY() == 5)
			mDown = false;
		else if (P->GetmY() + 1 == Players[i]->GetmY() && P->GetmX() == Players[i]->GetmX())
			mDown = false;

		if(P->GetmX() == 0)
			mLeft = false;
		else if (P->GetmX() - 1 == Players[i]->GetmX() && P->GetmY() == Players[i]->GetmY())
			mLeft = false;

		if (P->GetmX() == 5)
			mRight = false;
		else if (P->GetmX() == 5 || (P->GetmX() + 1 == Players[i]->GetmX() && P->GetmY() == Players[i]->GetmY()))
			mRight = false;
	}

	if (mUp == false && mDown == false && mLeft == false && mRight == false)
		return false;
	else
		return true;

}
bool Game::MovePhase(Player* P, int mAnswer)
{
	bool mMoveUp = false;
	bool mMoveDown = false;
	bool mMoveLeft = false;
	bool mMoveRight = false;


	for (unsigned int i = 0; i < Players.size(); ++i)
	{
		if (mAnswer == 1)
		{
			if (P->GetmY() == 0)
			{
				mMoveUp = false;
			}	
			else if (P->GetmY() - 1 == Players[i]->GetmY() && P->GetmX() == Players[i]->GetmX())
			{
				mMoveUp = false;
			}
			else if (!(P->GetmY() == 0 || (P->GetmY() - 1 == Players[i]->GetmY() && P->GetmX() == Players[i]->GetmX())))
			{
				mMoveUp = true;
			}
			/*break;*/
		}
		
		if (mAnswer == 2)
		{
			if (P->GetmY() == 5)
			{
				mMoveDown = false;
			}
			else if (P->GetmY() + 1 == Players[i]->GetmY() && P->GetmX() == Players[i]->GetmX())
			{
				mMoveDown = false;
			}
			else  if (!(P->GetmY() == 5 || (P->GetmY() + 1 == Players[i]->GetmY() && P->GetmX() == Players[i]->GetmX())))
			{
				mMoveDown = true;
			}
			/*break;*/
		}

		if (mAnswer == 3)
		{
			if (P->GetmX() == 0)
				mMoveLeft = false;
			else if (P->GetmX() - 1 == Players[i]->GetmX() && P->GetmY() == Players[i]->GetmY())
			{
				mMoveLeft = false;
			}
			else if (P->GetmX() != 0 && (P->GetmX() - 1 != Players[i]->GetmX() && P->GetmY() != Players[i]->GetmY()))
			{
				mMoveLeft = true;
			}
			/*break;*/
		}

		if (mAnswer == 4)
		{
			if (P->GetmX() == 5)
			{
				mMoveRight = false;
			}
			else if (P->GetmX() + 1 == Players[i]->GetmX() && P->GetmY() == Players[i]->GetmY())
			{
				mMoveRight = false;
			}
			if (!(P->GetmX() == 5 || (P->GetmX() + 1 == Players[i]->GetmX() && P->GetmY() == Players[i]->GetmY())))
			{
				mMoveRight = true;
			}
			/*break;*/
		}
	}

	if (mMoveUp == true)
	{
		P->SetmY(P->GetmY() - 1);
		return true;
	}
	else if (mMoveDown == true)
	{
		P->SetmY(P->GetmY() + 1);
		return true;
	}
	else if (mMoveLeft == true)
	{
		P->SetmX(P->GetmX() - 1);
		return true;
	}
	else if (mMoveRight == true)
	{
		P->SetmX(P->GetmX() + 1);
		return true;
	}
	else if (mMoveUp == false || mMoveDown == false || mMoveLeft == false || mMoveRight == false)
		return false;
		

}

bool Game::AttackPhase(Player* P, int mAnswer)
{
	/*for (unsigned int i = 0; i < Players.size(); ++i)
	{
		if (P->GetTeam() == 1)
		{
			if (P->GetType() == Player::Archer)
			{

			}
			else if (P->GetType() == Player::Warrior)
			{

			}
			else if (P->GetType() == Player::Sorcerer)
			{

			}
		}
		else if (P->GetTeam == 2)
		{
			if (P->GetType() == Player::Archer)
			{

			}
			else if (P->GetType() == Player::Warrior)
			{

			}
			else if (P->GetType() == Player::Sorcerer)
			{

			}
		}
	}*/
	return false;
}

Game::~Game()
{
	for (int i = 0; i < NumofPlayers; ++i)
	{
		delete Players[i];
	}
}
