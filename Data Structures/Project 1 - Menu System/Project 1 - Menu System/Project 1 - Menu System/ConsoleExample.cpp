#include <Windows.h>
#include <iostream>
#include <conio.h>
using namespace std;


int main(int argc, char ** argv)
{
	// make a handle and get a handle to the standard output window
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE); 
	COORD position = {18,0};

	// single colors
	SetConsoleTextAttribute(out, FOREGROUND_RED);
	cout << "Red\n";
	SetConsoleTextAttribute(out, FOREGROUND_RED | FOREGROUND_INTENSITY);
	cout << "Bright Red\n";
	SetConsoleTextAttribute(out, FOREGROUND_GREEN);
	cout << "Green\n";
	SetConsoleTextAttribute(out, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	cout << "Bright Green\n";
	SetConsoleTextAttribute(out, FOREGROUND_BLUE);
	cout << "Blue\n";
	SetConsoleTextAttribute(out, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	cout << "Bright Blue\n";

	// combination colors
	// 2s
	SetConsoleTextAttribute(out, FOREGROUND_RED | FOREGROUND_GREEN);
	cout << "Yellow\n";
	SetConsoleTextAttribute(out, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	cout << "Bright Yellow\n";
	SetConsoleTextAttribute(out, FOREGROUND_RED | FOREGROUND_BLUE);
	cout << "Magenta\n";
	SetConsoleTextAttribute(out, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	cout << "Bright Magenta\n";
	SetConsoleTextAttribute(out, FOREGROUND_GREEN | FOREGROUND_BLUE);
	cout << "Cyan\n";
	SetConsoleTextAttribute(out, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	cout << "Bright Cyan\n";
	// all 3
	SetConsoleTextAttribute(out, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	cout << "Grey\n";
	SetConsoleTextAttribute(out, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	cout << "White\n";


	SetConsoleTextAttribute(out, BACKGROUND_GREEN | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	cout << "green background and white text\n";
	SetConsoleTextAttribute(out, 0 | BACKGROUND_GREEN );
	cout << "green background and black text\n";



	SetConsoleTextAttribute(out, 0);
	cout << "you can't read this because it is black text on a black background\n";


	




	
	// make sure the 'press any key blah blah' is at the bottom and not overwriting any text
	position.X = 0;
	position.Y = 22;
	SetConsoleCursorPosition(out, position);










	SetConsoleTextAttribute(out, 0 | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	cout << "back to default\n";

	return 0;
}