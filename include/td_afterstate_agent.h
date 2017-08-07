#ifndef TD_AFTERSTATE_AGENT_H
#define TD_AFTERSTATE_AGENT_H

#include "state_value_agent.h"
#include "ntuple2.h"

class TDAfterstateAgent : public StateValueAgent {
    private :
        int choose_greedy_action(Board2048 const& state) const;

    public :
        TDAfterstateAgent(NTuple2 *ntuple, double lr) :
            StateValueAgent(ntuple, lr) {
        }

        int choose_action(Board2048 const& state, double eps=0.001) const;

        double state_value(Board2048 const& state, int action) const;

        void learn(Board2048 const& state, double reward, 
                Board2048 const& after_state, Board2048 const& next_state);
};

#endif // TD_AFTERSTATE_AGENT_H
