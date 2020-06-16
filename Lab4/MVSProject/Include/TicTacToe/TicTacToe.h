#pragma once

#include <random>
#include <vector>
#include <array>
#include <algorithm>
#include <map>

namespace TicTacToe{

	#define CHARX 'X'
	#define CHARO 'O'
	#define CHARSPACE ' '
	#define DELIMV '|'
	#define DELIMH "----------------"
	#define FIELDSIZE 3

	enum Status {
		PLAY,
		USER_WIN,
		BOT_WIN,
		NOT_WIN
	};
	struct Game {
		char bord[FIELDSIZE][FIELDSIZE];
		bool isUserTurn;
		char userChar;
		char botChar;
		Status status;
	};

	Game initGame(_In_ char userChar);

	void updateDisplay(_In_ const Game game);

	void botTurn(_Inout_ Game* game);

	void userTurn(_Inout_ Game* game);

	bool updateGame(_Inout_ Game* game);
}