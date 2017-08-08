#include "board.h"
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

#include <cassert>

struct episode_result {
    double score;
    bool is_game_won;
    int max_tile;
};

bool is_game_won(Board2048 const& state) {
    for (int x = 0; x < 4; ++x) {
        for (int y = 0; y < 4; ++y) {
            if (state(x, y) >= 11) {
                return true; 
            } 
        } 
    }
    return false;
}

int get_max_tile(Board2048 const& state) {
    int max_tile = 0;

    for (int x = 0; x < 4; ++x) {
        for (int y = 0; y < 4; ++y) {
            if (state(x, y) > max_tile) {
                max_tile = state(x, y); 
            } 
        } 
    }
 
    return max_tile;
}

episode_result playSingleEpisode(StateValueAgent & agent, double eps) {
	int score = 0;
    Board2048 state;
    state.add_random_tile();
    state.add_random_tile();

	while (!state.is_game_over()) {
        //state.print();

        int action = agent.choose_action(state);

        Board2048 after_state(state);
        after_state.make_move(action);

        assert(state != after_state);

        Board2048 next_state(after_state);
        next_state.add_random_tile();
		
        assert(next_state != after_state);
        
        int reward = after_state.get_reward();
		
        agent.learn(state, after_state, next_state, action, reward);

		score += reward;
		state = next_state;
	}

    episode_result result;
    result.score = (double)score;
    result.is_game_won = is_game_won(state);
    result.max_tile = get_max_tile(state);
	return result;
}

void learn(int print_every, double lr, int save_every, 
        std::string const& save_dir = "") {
    NTuple2 *ntuple = new NTuple2(lr);
    TDAfterstateAgent agent(ntuple, 0.001);

    double avg_score = 0.;
    int best_score = 0;
    int worst_score = 999999;
    int games_won = 0;

	int game_num = 0;
	while (true) {
        game_num++;

        episode_result result = playSingleEpisode(agent, 0.001);

        if (result.score < worst_score) { worst_score = result.score; }
        if (result.score > best_score) { best_score = result.score; }
        avg_score += result.score;
        
        if (result.is_game_won) {
            games_won++; 
        }

        if (game_num % print_every == 0) {
            avg_score /= double(print_every);

            std::cout << "GAMES PLAYED = " << game_num << "\n";
            std::cout << "avg score = " << avg_score << "\n";
            std::cout << "best score = " << best_score << "\n";
            std::cout << "worst score = " << worst_score << "\n";
            std::cout << "games won = " << games_won << "\n";
            std::cout << "\n";

            avg_score = 0.;
            best_score = 0;
            worst_score = 999999;
            games_won = 0;
		}
        
        if (game_num % save_every == 0 && save_dir != "") {
            std::string filepath = save_dir + "/model";
            filepath += std::to_string(game_num / save_every) + ".ntuple2";
            ntuple->save(filepath); 
        }
    }
}

int main(int argc, char** argv) {
    srand(time(NULL));

    if (argc != 4) {
        printf("Usage %s print_every learning_rate save_every\n", argv[0]);
        return 1;
    }
    
    learn(atoi(argv[1]), atof(argv[2]), atoi(argv[3]), "models");
	return 0;
}
