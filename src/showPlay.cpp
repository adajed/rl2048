#include "game.h"
#include "policy.h"

#include "ntuple.h"
#include "ntuple1.h"
#include "ntuple2.h"

#include <iostream>
#include <unistd.h>

void play(NTuple* p_ntuple, double sleepTime) {
	int** board = getNewBoard();
	addRandomTile(board);
	addRandomTile(board);

	int score = 0;

	while (!isGameOver(board)) {
		printBoard(board);
		std::cout << "\n";

		int m = greedyPolicyTDA(p_ntuple, board);
		int** b = makeMove(board, m);
		addRandomTile(b);
		score += getReward();

		deleteBoard(board);
		board = b;

		sleep(sleepTime);
	}
	printBoard(board);

	std::cout << "score = " << score << "\n";
}

int main(int argc, char* argv[]) {
	if (argc != 4) {
		return 1;
	}

	NTuple* p;
	if (atoi(argv[1]) == 1) {
		p = new NTuple1(argv[2]);
	} else {
		p = new NTuple2(argv[2]);
	}
	
	play(p, atof(argv[3]));
	delete p;

	return 0;
}
