#ifndef POLICY_H
#define POLICY_H

#include "game.h"
#include "ntuple.h"

#include <iostream>

long double evaluateTD(const NTuple* p_ntuple, int** board, int move) {
	int** new_board = makeMove(board, move);
	int reward = getReward();

	double sum1 = 0;
	double sum2 = 0;
	int i = 0;
	for (int x = 0; x < 4; ++x) {
		for (int y = 0; y < 4; ++y) {
			if (new_board[x][y] == 0) {
				i++;
				new_board[x][y] = 1;
				sum1 += p_ntuple->getValue(new_board);

				new_board[x][y] = 2;
				sum2 += p_ntuple->getValue(new_board);

				new_board[x][y] = 0;
			}
		}
	}

	deleteBoard(new_board);

	return reward + (0.9 * sum1 + 0.1 * sum2) / double(i);

}

long double evaluateTDA(const NTuple* p_ntuple, int** board, int move) {
	int** newBoard = makeMove(board, move);
	int reward = getReward();

	long double ret = reward + p_ntuple->getValue(newBoard);
	deleteBoard(newBoard);

	return ret;
}

extern int greedyPolicyTD(const NTuple* p_ntuple, int** board) {
	int move = 0;
	long double max = 0;
	bool p = false;

	for (int m = 0; m < 4; m++) {
		if (isMoveAvailable(board, m)) {
			long double v = evaluateTD(p_ntuple, board, m);
			if (v > max || !p) {
				move = m;
				max = v;
				p = true;
			}
		}
	}

	return move;
}


extern int greedyPolicyTDA(const NTuple* p_ntuple, int** board) {
	int move = 0;
	long double max = 0;
	bool p = false;

	for (int m = 0; m < 4; m++) {
		if (isMoveAvailable(board, m)) {
			long double v = evaluateTDA(p_ntuple, board, m);
			if (v > max || !p) {
				move = m;
				max = v;
				p = true;
			}
		}
	}

	return move;
}

extern int epsilonGreedyPolicyTDA(const NTuple* p_ntuple, double epsilon, int** board) {
	int move;
	if ((double(rand()) / RAND_MAX) < epsilon) {
		do {
			move = rand() % 4;
		} while (!isMoveAvailable(board, move));
	} else {
		move = greedyPolicyTDA(p_ntuple, board);
	}

	return move;
}

extern void learnTD(NTuple* p_ntuple, long double alpha, 
		int** board1, int** board2, int** board3, 
		int move, int reward) {
	p_ntuple->learn(board1, reward + p_ntuple->getValue(board3) - p_ntuple->getValue(board1), alpha);
}


extern void learnTDA(NTuple* p_ntuple, long double alpha, 
		int** board1, int** board2, int** board3, 
		int move, int reward) {
	int nextMove = greedyPolicyTDA(p_ntuple, board3);
	int** newBoard = makeMove(board3, nextMove);
	int nextReward = getReward();

	p_ntuple->learn(board2, (long double)nextReward + p_ntuple->getValue(newBoard) - p_ntuple->getValue(board2), alpha);
	
	deleteBoard(newBoard);
}

#endif // POLICY_H
