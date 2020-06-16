#include "TicTacToe.h"
#include "../HumanCode/HumanCode.h"

using namespace std;
using namespace HumanCode;

namespace TicTacToe{
	std::random_device device;
	std::mt19937 RndGen(device());
	std::uniform_int_distribution<uint32_t> Dist(0,1);

	Game initGame(_In_ char userChar) {
		Game GameStruct;
		GameStruct.userChar = userChar;
		
		if (GameStruct.userChar == CHARX)
			GameStruct.botChar = CHARO;
		else
			GameStruct.botChar = CHARX;

		GameStruct.isUserTurn = !Dist(RndGen);
		for (size_t i = 0; i < FIELDSIZE*FIELDSIZE; i++)
		{
			*((char*)GameStruct.bord + i) = CHARSPACE;
		}
		updateDisplay(GameStruct);
		return GameStruct;
	}

	void updateDisplay(_In_ const Game game){
		HCConsole::CLS();
		printf("   %c %i %c %i %c %i %c\n%s\n", DELIMV, 1, DELIMV, 2, DELIMV, 3, DELIMV, DELIMH);
		for (size_t i = 0; i < 3; i++)
		{
			printf(" %i %c %c %c %c %c %c %c\n%s\n", 
					i+1, DELIMV, game.bord[i][0], DELIMV, game.bord[i][1], DELIMV, game.bord[i][2], DELIMV, DELIMH);
		}
	}

	void userTurn(_Inout_ Game * game) {
		UINT UserColumn, UserRaw;
		cout << "Your turn!" << endl;
		do{
			do{
				cout << "Choose raw[1," << FIELDSIZE << "]:";
				cin >> UserRaw; UserRaw--;
				if (UserRaw >= 0 and UserRaw <= FIELDSIZE-1) break;
				cout << "Asshole, can you choose number between 1 and <<" << FIELDSIZE << "? Try again.\n ";
			} while (true);
			do {
				cout << "Choose column[1," << FIELDSIZE << "]:";
				cin >> UserColumn; UserColumn--;
				if (UserColumn >= 0 and UserColumn <= FIELDSIZE - 1) break;
				cout << "Asshole, can you choose number between 1 and " << FIELDSIZE << "? Try again.\n ";
			} while (true);
			if (game->bord[UserRaw][UserColumn] == CHARSPACE) break;
			std::cout << "\nOmg, this cell is already taken, man.";
		} while (true);
		game->bord[UserRaw][UserColumn] = game->userChar;
	}
	
	void botTurn(_Inout_ Game* game){

		struct line {
			int scores = 0;
			std::vector<char*> freecells;
		};

		struct {
			struct {
				line lines[FIELDSIZE];
			}rows;

			struct {
				line lines[FIELDSIZE];
			}cols;

			struct {
				line lines[2];
			}dias;
		} linesByType;

		auto ScoreExpression = [game](line &LineObj,int i, int j) {
			if (game->bord[i][j] == game->userChar) LineObj.scores += 1;
			if (game->bord[i][j] == CHARSPACE) LineObj.freecells.push_back(&game->bord[i][j]);
		};

		for (int i = 0; i < FIELDSIZE; i++) {
			for (int j = 0; j < FIELDSIZE; j++) {
				ScoreExpression(linesByType.rows.lines[i], i, j);

				ScoreExpression(linesByType.cols.lines[i], j, i);
			}

			ScoreExpression(linesByType.dias.lines[0], i, i);
			ScoreExpression(linesByType.dias.lines[1], i, FIELDSIZE - i - 1);
		}

		vector<line> allLinesList;

		for (auto &it : linesByType.cols.lines)
			allLinesList.push_back(it);

		for (auto& it : linesByType.rows.lines)
			allLinesList.push_back(it);

		for (auto& it : linesByType.dias.lines)
			allLinesList.push_back(it);

		auto SortMethod = [](line F, line S) {
			if (F.scores > S.scores) return true;
			return false;
		};


		int totalScores = 0;

		for (auto& it : allLinesList) {
			totalScores += it.scores;
			cout << it.scores;
		}

		std::sort(allLinesList.begin(), allLinesList.end(), SortMethod);

		if (!totalScores){
			int centerCell = floor(FIELDSIZE / 2);
			game->bord[centerCell][centerCell] = game->botChar;
			cout << totalScores;
			return;
		}

		for (auto &priorityLine :allLinesList){
			if (priorityLine.freecells.size()){
				std::random_device device;
				std::mt19937 RndGen(device());
				std::uniform_int_distribution<uint32_t> RandCell(0, priorityLine.freecells.size() - 1);
				char* priorityCell = priorityLine.freecells.at(RandCell(RndGen));
				*priorityCell = game->botChar;
				return;
			}
		}

	}

	bool updateGame(_Inout_ Game* game) {
		updateDisplay(*game);

		game->isUserTurn = !game->isUserTurn;

		struct counterAdvanceT {
			int User = 0;
			int Bot = 0;
		};

		struct {
			int CounterGlobal = 0;
			counterAdvanceT Rows[FIELDSIZE];
			counterAdvanceT Cols[FIELDSIZE];
			counterAdvanceT Dias[2];
		} Counters;

		auto CollectExpression = [game, &Counters](int i, int j, counterAdvanceT& certainCounter) {
			if (game->bord[i][j] == game->botChar)	certainCounter.Bot++;
			if (game->bord[i][j] == game->userChar) certainCounter.User++;
			if (game->bord[i][j] == CHARSPACE)		Counters.CounterGlobal++;
		};

		for (int i = 0; i < FIELDSIZE; i++) {
			for (int j = 0; j < FIELDSIZE; j++) {
				CollectExpression(i, j, Counters.Rows[i]);
				CollectExpression(i, j, Counters.Cols[j]);
			}

			CollectExpression(i, i, Counters.Dias[0]);
			CollectExpression(i, FIELDSIZE - i - 1, Counters.Dias[1]);
		}

		auto CheckWinExpression = [&game](counterAdvanceT line) -> bool {
			if (line.Bot == FIELDSIZE) {
				game->status = Status::BOT_WIN;
				return true;
			}
			if (line.User == FIELDSIZE) {
				game->status = Status::USER_WIN;
				return true;
			}
			return false;
		};

		for (auto line : Counters.Rows)
			if (CheckWinExpression(line)) return true;

		for (auto line : Counters.Cols)
			if (CheckWinExpression(line)) return true;

		for (auto line : Counters.Dias)
			if (CheckWinExpression(line)) return true;

		if (!Counters.CounterGlobal) {
			game->status = Status::NOT_WIN;
			return true;
		}

		return false;
	}
}