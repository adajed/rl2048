#ifndef DUMMY_AGENT_H
#define DUMMY_AGENT_H

#include "state_value_agent.h"

/*
 * dummy agent for testing
 */
class DummyAgent : public StateValueAgent {
    public :
        DummyAgent() :
            StateValueAgent(0.) {
        }

        int choose_action(Board2048 const& state, double eps=0.001) const {
            int action = 0;
            while (!state.is_move_possible(action)) {
                action++;
            }
            
            return action;
        }

        double state_value(Board2048 const& state, int action) const {
            return 10.;
        }

        void learn(Board2048 const& state, double reward,
                Board2048 const& after_state, Board2048 const& next_state) {
        }
};

#endif // DUMMY_AGENT_H
