#ifndef MODEL_H
#define MODEL_H

#include "board.h"

/*
 * base class for model
 */
class Model {
    public:
        /*
         * Method for calculating value of state
         */
        virtual double get_value(Board2048 const& state) const = 0;

        /*
         * method for updating model base on transition
         *
         * state - board, for which update is being made
         * new_value - new value calculated for state based on transition
         */
        virtual void learn(Board2048 const& state, double new_value) = 0;
};

#endif // MODEL_H
