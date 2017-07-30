#include "board.h"
#include "ntuple.h"
#include "ntuple2.h"
#include "state_value_agent.h"
#include "td_afterstate_agent.h"

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <cmath>

#include <vector>
#include <unistd.h>

int playSingleEpisode(StateValueAgent & agent, double eps) {
	int score = 0;
    Board2048 state;

	while (!state.is_game_over()) {
        int action = agent.choose_action(state, eps);
		
        Board2048 after_state(state);
        after_state.make_move(action);

        Board2048 next_state(after_state);
        next_state.add_random_tile();
		
        int reward = after_state.get_reward();
		
        agent.learn(state, reward, after_state, next_state);

		score += reward;
		state = next_state;
	}

	return score;
}

void learn(int printAfter, int saveAfter, char* fileName, char* dir, double alpha, int type) {
	NTuple* p_ntuple = new NTuple2;
    /*
	if (type == 1) {
		if (fileName == nullptr) {
			p_ntuple = new NTuple2;
		} else {
			try {
				p_ntuple = new NTuple2(fileName);
			} catch (int n) {
				std::cerr << "file \"" << fileName << "\" not found\n";
				return;
			}
		}
	} else if (type == 2) {
		if (fileName == nullptr) {
			p_ntuple = new NTuple2;
		} else {
			try {
				p_ntuple = new NTuple2(fileName);
			} catch (int n) {
				std::cerr << "file \"" << fileName << "\" not found\n";
				return;
			}
		}
	} else {
		std::cerr << "wrong ntuple type, should be 1 or 2\n";
		return;
	}
    */

    TDAfterstateAgent agent(p_ntuple);

	int gameNum = 1;
	while (true) {
		double avgScore = 0;
		int bestScore = 0;
		int worstScore = 999999;

		for (int i = 0; i < printAfter; i++) {
			int score = playSingleEpisode(agent, 0.001);

			if (score < worstScore) { worstScore = score; }
			if (score > bestScore) { bestScore = score; }
			avgScore += (double)score;

			gameNum++;
		}

		avgScore /= double(printAfter);

		std::cout << "GAMES PLAYED = " << gameNum << "\n";
		std::cout << "avg score = " << avgScore << "\n";
		std::cout << "best score = " << bestScore << "\n";
		std::cout << "worst score = " << worstScore << "\n";
		std::cout << "\n";
	}

	delete p_ntuple;
}

/*
* arguments :
* 1 - po ilu grach wypisywac statystyki
* 2 - po ilu grach zapisywać
* 3 - folder do zapisywania
* 4 - alpha
* 5 - typ tupla
* 6 - poczatkowy tuple, jesli nie podany zaczyna od zera
*/
int main(int argc, char** argv) {
	srand(time(NULL));


	if (argc == 6) {
		learn(atoi(argv[1]), atoi(argv[2]), nullptr, argv[3], atof(argv[4]), atoi(argv[5]));
	} else if (argc == 7) {
		learn(atoi(argv[1]), atoi(argv[2]), argv[6], argv[3], atof(argv[4]), atoi(argv[5]));
	} else {
		std::cout << "wrong number of args\n";
	}

	return 0;
}
