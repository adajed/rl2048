#ifndef TD_AFTERSTATE_AGENT_H
#define TD_AFTERSTATE_AGENT_H

#include "state_value_agent.h"
#include "ntuple2.h"

class TDAfterstateAgent : public StateValueAgent {
    private:
        double eps;

        int choose_greedy_action(Board2048 const& state) const;

    public :
        TDAfterstateAgent(Model *model, double eps) :
            StateValueAgent(model), eps(eps) {
        }

        int choose_action(Board2048 const& state) const;

        double state_value(Board2048 const& state) const;

        void learn(Board2048 const& state, Board2048 const& after_state, 
                Board2048 const& next_state, int action, double reward);
};

#endif // TD_AFTERSTATE_AGENT_H
