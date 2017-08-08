#ifndef NTUPLE1_H
#define NTUPLE1_H

#include <fstream>
#include <vector>
#include <string>

#include "model.h"

typedef std::pair<int, int> pairii;
typedef std::vector<pairii> vecpii;

/*
 * Implemetantation of first N-tuple network architecture
 *
 * Consist of 4 horizontal, 4 vertical and 9 square (2 by 2) tuples.
 * Assumes each tile has 15 possible states (maximal tile can be 2^14 = 16 384).
 *
 * Has 17 * 15^4 = 860 526 weights.
 */
class NTuple1 : public Model {
	private:
        const static int numStates = 15;
        const static int len4 = 50625;
        const static int len6 = 11390625;

        std::vector<vecpii> tuples;
        long double weights[17][len4];
        
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
            for (int i = 0; i < 4; ++i) {
                tuples.push_back(vecpii());
                tuples.push_back(vecpii());
                for (int k = 0; k < 4; ++k) {
                    tuples[2 * i].push_back(std::make_pair(i, k));
                    tuples[2 * i + 1].push_back(std::make_pair(k, i));
                }
            }

            for (int x = 0; x < 3; ++x) {
                for (int y = 0; y < 3; ++y) {
                    tuples.push_back(vecpii());
                    tuples[8 + 3 * x + y].push_back(std::make_pair(x, y));
                    tuples[8 + 3 * x + y].push_back(std::make_pair(x + 1, y));
                    tuples[8 + 3 * x + y].push_back(std::make_pair(x, y + 1));
                    tuples[8 + 3 * x + y].push_back(std::make_pair(x + 1, y + 1));
                }
            }
        }

	public:
        /*
         * constructs new NTuple1 
         * all weights are initialized to 0
         */
        NTuple1(double lr) : tuples(), lr(lr) {
            init_tuples();
            for (int t = 0; t < 17; t++) {
                for (int i = 0; i < len4; i++) {
                    weights[t][i] = 0.0;
                }
            }

        }

        /*
         * read NTuple1 from file
         *
         * filepath - binary file with NTuple1 weights
         */
        NTuple1(std::string const& filepath, double lr) : lr(lr) {
            init_tuples();

            std::ifstream file(filepath.c_str(), std::ios::in | std::ios:binary);
            if (!file.is_open()) {
                throw -1;
            }

            for (int t = 0; t < 17; t++) {
                for (int i = 0; i < len4; i++) {
                    file.read(&weights[t][i], sizeof(weights[t][i]));
                }
            }
        
            file.close();
        }
        
        double get_value(Board2048 const& state) const {
            double value = 0.;

            for (int t = 0; t < 17; ++t) {
                int index = get_index(state, tuples[t]);
                value += weights[t][index];
            }
            
            return value;
        }

        void learn(Board2048 const& state, double delta) {
            for (int t = 0; t < 17; ++t) {
                int index = get_index(state, tuples[t]);
                weights[t][index] += delta * lr;
            }
        }

        /*
         * Saves NTuple1 weights to binary file
         *
         * filepath - filepath, where NTuple1 will be saved
         */
        void save(std::string const& filepath) {
            std::ofstream file(filepath.c_str(), std::ios::binary | std::ios::out);
            if (!file.is_open()) {
                // TODO: throw exception
                throw -1;
            }

            for (int t = 0; t < 17; t++) {
                for (int i = 0; i < len4; i++) {
                    file.write(weights[t][i], sizeof(weights[t][i]))
                }
            }
        
            file.close();
        }

};

#endif // NTUPLE1_H
