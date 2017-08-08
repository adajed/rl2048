#include "td_afterstate_agent.h"

#include "cassert"

int TDAfterstateAgent::choose_greedy_action(Board2048 const& state) const {
	int best_action = 0;
	double value, best_value = 0.;
	bool p = false;

	for (int m = 0; m < 4; m++) {
		if (state.is_move_possible(m)) {
            Board2048 temp_state(state);
            temp_state.make_move(m);

			value = state_value(temp_state);
			if (value > best_value || !p) {
				best_action = m;
				best_value = value;
				p = true;
			}
		}
	}

	return best_action;
}

int TDAfterstateAgent::choose_action(Board2048 const& state) const {
	int action;
	if ((double(rand()) / RAND_MAX) < eps) {
		do {
			action = rand() % 4;
		} while (!state.is_move_possible(action));
	} else {
		action = choose_greedy_action(state);
	}

	return action;
}

double TDAfterstateAgent::state_value(Board2048 const& state) const {
	int reward = state.get_reward();

    double ret = (double)reward + model->get_value(state);
    return ret;
}


void TDAfterstateAgent::learn(Board2048 const& state, Board2048 const& after_state, 
        Board2048 const& next_state, int action, double reward) {
    if (!next_state.is_game_over()) {
        int next_action = choose_greedy_action(next_state);
        
        Board2048 temp_state(next_state);
        temp_state.make_move(next_action);
        int next_reward = temp_state.get_reward();

        assert(next_state != temp_state);

        double temp_state_value = model->get_value(temp_state);
        model->learn(after_state, (double)next_reward + temp_state_value);
    } else {
        model->learn(after_state, 0.);
    }
}

