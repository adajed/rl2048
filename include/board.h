#ifndef GAME_H
#define GAME_H

#include <cmath>
#include <cstdlib>
#include <iostream>

class Board2048 {
    private :
        int **state;
        int reward;

        /*
         * allocates new 4x4 int array
         */
        int **new_2D_array() const;
        
        void free_state(int **state) const;

        /*
         * rotate board counter clock-wise
         */
        int **rotate_state(int r) const;
    
    public :
        Board2048();
        Board2048(Board2048 const& other);

        int& operator()(int x, int y) {
            return state[x][y];
        }

        int& operator()(std::pair<int, int> p) {
            return state[p.first][p.second];
        }

        int const& operator()(int x, int y) const {
            return state[x][y];
        }

        int const& operator()(std::pair<int, int> p) const {
            return state[p.first][p.second];
        }


        /*
         * makes move
         *
         * assumes that given move is possible
         */
        void make_move(int action);

        /*
         * adds new tile in random empty place
         * 1 with 0.9 probabilty and
         * 2 with 0.1 probabilty
         *
         * assumes that there is empty place on board
         */
        void add_random_tile();

        /*
         * checks if action is possible in current state
         */
        bool is_move_possible(int action) const;

        bool is_game_over() const;

        int get_reward() {
            return reward;
        }

        ~Board2048();
};

#endif // GAME_H