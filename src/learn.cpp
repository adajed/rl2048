#include "board.h"
#include "ntuple.h"
#include "ntuple2.h"
#include "state_value_agent.h"
#include "td_afterstate_agent.h"
#include "dummy_agent.h"

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <cmath>

#include <vector>
#include <unistd.h>

#include <cassert>

int playSingleEpisode(TDAfterstateAgent & agent, double eps) {
	int score = 0;
    Board2048 state;
    state.add_random_tile();
    state.add_random_tile();

	while (!state.is_game_over()) {
        //state.print();

        int action = agent.choose_action(state, eps);

        Board2048 after_state(state);
        after_state.make_move(action);

        assert(state != after_state);

        Board2048 next_state(after_state);
        next_state.add_random_tile();
		
        assert(next_state != after_state);
        
        int reward = after_state.get_reward();
		
        agent.learn(state, reward, after_state, next_state);

		score += reward;
		state = next_state;
	}
    //state.print();

	return score;
}

void learn(int printAfter, double alpha) {
    NTuple2 *ntuple = new NTuple2;
    TDAfterstateAgent agent(ntuple, 0.01);

	int gameNum = 1;
	while (true) {
		double avgScore = 0;
		int bestScore = 0;
		int worstScore = 999999;

		for (int i = 0; i < printAfter; i++) {
            // std::cout << gameNum << std::endl;
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
}

int main(int argc, char** argv) {
    srand(time(NULL));

    if (argc != 3) {
        printf("Usage %s print_every learning_rate\n", argv[0]);
        return 1;
    }
    
    learn(atoi(argv[1]), atof(argv[2]));
	return 0;
}
