#ifndef STATE_VALUE_AGENT_H
#define STATE_VALUE_AGENT_H

#include "ntuple.h"
#include "board.h"

class StateValueAgent {
    protected :
        NTuple *ntuple;
        double lr;

    public :
        StateValueAgent(NTuple *ntuple, double lr = 0.001) {
            ntuple = ntuple;
            lr = lr;
        }

        virtual int choose_action(Board2048 const& state, double eps=0.001) const = 0;

        virtual double state_value(Board2048 const& state, int action) const = 0;

        virtual void learn(Board2048 const& state, double reward, 
                Board2048 const& after_state, Board2048 const& next_state) = 0;
};

#endif // STATE_VALUE_AGENT_H
