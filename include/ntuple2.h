#ifndef NTUPLE2_H
#define NTUPLE2_H

#include <fstream>
#include <vector>

#include "model.h"

typedef std::pair<int, int> pairii;
typedef std::vector<pairii> vecpii;

/*
 * Implementation of N-tuple network architecture from second experiment
 *
 * Consist of 2 streight and 2 rectangular (2 by 3) tuples.
 * Also each tuple is aplied to 8 different rotations and reflections of a board.
 * Assumes each tile has 15 possible states (maximal tile can be 2^14 = 16 384).
 *
 * Has 2 * 15^4 + 2 * 15^6 = 22 882 500 weights.
 */
class NTuple2 : public Model {
	private:
        const static int numStates = 15;
        const static int len4 = 50625;
        const static int len6 = 11390625;

        std::vector<vecpii> tuples;
        long double weights4[2][len4];
        long double weights6[2][len6];
        
        double lr;

        /*
         * calculates index of state in given tuple
         */
        int get_index(Board2048 const& state, vecpii const& tuple) const {
            int res = 0;
            int pow = 1;
            for (auto it = tuple.begin(); it < tuple.end(); ++it) {
                res += state(*it) * pow;
                pow *= numStates;
            }
            return res;
        }

        /* 
         * creates all tuples
         */
        void init_tuples() {
            for (int i = 0; i < 2; ++i) {
                tuples.push_back(vecpii());
                for (int x = 0; x < 4; ++x) {
                    tuples[i].push_back(std::make_pair(x, i));
                }
            }

            for (int i = 0; i < 2; ++i) {
                tuples.push_back(vecpii());
                for (int x = 0; x < 3; ++x) {
                    tuples[2 + i].push_back(std::make_pair(x, i));
                    tuples[2 + i].push_back(std::make_pair(x, i + 1));
                }
            }
        }

        /*
         * calculates all symmetries for given state
         */
        std::vector<Board2048> get_symmetries(Board2048 const& state) const {
            std::vector<Board2048> symmetries;
            for (int i = 0; i < 8; ++i) {
                symmetries.push_back(Board2048(state));
            }

            for (int x = 0; x < 4; ++x) {
                for (int y = 0; y < 4; ++y) {
                    int val = state(x, y);
                    symmetries[0](x, y) = val;
                    symmetries[1](3 - x, y) = val;
                    symmetries[2](x, 3 - y) = val;
                    symmetries[3](3 - x, 3 - y) = val;
                    symmetries[4](y, x) = val;
                    symmetries[5](y, 3 - x) = val;
                    symmetries[6](3 - y, x) = val;
                    symmetries[7](3 - y, 3 - x) = val;
                }
            }

            return symmetries;
        }

	public:
        /*
         * constructs new NTuple2 
         * all weights are initialized to 0
         */
        NTuple2(double lr) : tuples(), lr(lr) {
            init_tuples();
            
            for (int t = 0; t < 2; t++) {
                for (int i = 0; i < len4; i++) {
                    weights4[t][i] = 0.0;
                }
            }
            for (int t = 0; t < 2; t++) {
                for (int i = 0; i < len6; i++) {
                    weights6[t][i] = 0.0;
                }
            }
        
        }
 
        /*
         * read NTuple2 from file
         *
         * filepath - binary file with NTuple2 weights
         */
        NTuple2(std::string const& filepath, double lr) : lr(lr) {
            init_tuples();

            std::ifstream file(filepath.c_str(), std::ios::in | std::ios::binary);
            if (!file.is_open()) {
                throw -1;
            }

            for (int t = 0; t < 2; t++) {
                for (int i = 0; i < len4; i++) {
                    file.read((char *)&weights4[t][i], sizeof(weights4[t][i]));
                }
            }
            for (int t = 0; t < 2; t++) {
                for (int i = 0; i < len6; i++) {
                    file.read((char *)&weights6[t][i], sizeof(weights6[t][i]));
                }
            }
        
            file.close();
        }
       
        double get_value(Board2048 const& state) const {
            double value = 0.0;
            std::vector<Board2048> symmetries = get_symmetries(state);

            for (int t = 0; t < 2; ++t) {
                for (auto const& s : symmetries) {
                    int index = get_index(s, tuples[t]);
                    value += weights4[t][index];
                }
            }
            for (int t = 0; t < 2; ++t) {
                for (Board2048 const& s : symmetries) {
                    int index = get_index(s, tuples[2 + t]);
                    value += weights6[t][index];
                }
            }
            return value;
        }

        void learn(Board2048 const& state, double new_value) {
            std::vector<Board2048> symmetries = get_symmetries(state);
    
            double delta = new_value - get_value(state);

            for (int t = 0; t < 2; ++t) {
                for (Board2048 const& s : symmetries) {
                    int index = get_index(s, tuples[t]);
                    weights4[t][index] += delta * lr;
                }
            }
            for (int t = 0; t < 2; ++t) {
                for (Board2048 const& s : symmetries) {
                    int index = get_index(s, tuples[2 + t]);
                    weights6[t][index] += delta * lr;
                }
            }
        }

        /*
         * Saves NTuple2 weights to binary file
         *
         * filepath - filepath, where NTuple2 will be saved
         */
        void save(std::string const& filepath) {
            std::ofstream file(filepath.c_str(), std::ios::binary | std::ios::out);
            if (!file.is_open()) {
                // TODO: throw exception
                throw -1;
            }

            for (int t = 0; t < 2; t++) {
                for (int i = 0; i < len4; i++) {
                    file.write((char *) &weights4[t][i], sizeof(weights4[t][i]));
                }
            }
            for (int t = 0; t < 2; t++) {
                for (int i = 0; i < len6; i++) {
                    file.write((char *) &weights6[t][i], sizeof(weights6[t][i]));
                }
            }
        
            file.close();
        }

};

#endif // NTUPLE2_H
