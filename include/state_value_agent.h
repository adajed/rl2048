#ifndef STATE_VALUE_AGENT_H
#define STATE_VALUE_AGENT_H

#include "model.h"
#include "board.h"

/*
 * Base class for state value agents
 */
class StateValueAgent {
    protected :
        Model *model;

    public :
        StateValueAgent(Model *model) :
            model(model) {
        }

        /*
         * chooses action to take in given state
         */
        virtual int choose_action(Board2048 const& state) const = 0;

        /*
         * calculates value for given state
         */
        virtual double state_value(Board2048 const& state) const = 0;

        /*
         * updates model based on transition
         */
        virtual void learn(Board2048 const& state, Board2048 const& after_state, 
                Board2048 const& next_state, int action, double reward) = 0;
};

#endif // STATE_VALUE_AGENT_H
