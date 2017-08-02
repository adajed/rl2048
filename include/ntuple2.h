#ifndef NTUPLE2_H
#define NTUPLE2_H

#include <fstream>
#include <iostream>

#include <vector>

#include "ntuple.h"
#include "board.h"

typedef std::pair<int, int> pairii;
typedef std::vector<pairii> vecpii;

class NTuple2 : public NTuple {
	private:
        const static int numStates = 15;

        const static int len4 = 50625;
        const static int len6 = 11390625;

        std::vector<vecpii> tuples;
        
        long double wages4[2][len4];
        long double wages6[2][len6];
        
        int get_index(Board2048 const& state, vecpii const& tuple) const {
            int res = 0;
            int pow = 1;
            for (auto it = tuple.begin(); it < tuple.end(); ++it) {
                res += state(*it) * pow;
                pow *= numStates;
            }
            return res;
        }

        void initTuples() {
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

        std::vector<Board2048> get_symmetries(Board2048 const& state) const {
            std::vector<Board2048> symmetries;
            for (int i = 0; i < 8; ++i) {
                symmetries.push_back(Board2048(state));
            }

            for (int x = 0; x < 4; ++x) {
                for (int y = 0; y < 4; ++y) {
                    symmetries[0](x, y) = state(x, y);
                    symmetries[1](3 - x, y) = state(x, y);
                    symmetries[2](x, 3 - y) = state(x, y);
                    symmetries[3](3 - x, 3 - y) = state(x, y);
                    symmetries[4](y, x) = state(x, y);
                    symmetries[5](y, 3 - x) = state(x, y);
                    symmetries[6](3 - y, x) = state(x, y);
                    symmetries[7](3 - y, 3 - x) = state(x, y);
                }
            }

            return symmetries;
        }

	public:
        NTuple2(char *fileName) {
            initTuples();

            std::ifstream file(fileName);
            if (!file.is_open()) {
                throw -1;
            }

            for (int t = 0; t < 2; t++) {
                for (int i = 0; i < len4; i++) {
                    file >> wages4[t][i];
                }
            }
            for (int t = 0; t < 2; t++) {
                for (int i = 0; i < len6; i++) {
                    file >> wages6[t][i];
                }
            }
        
            file.close();
        }

        NTuple2() : tuples() {
            initTuples();
            
            for (int t = 0; t < 2; t++) {
                for (int i = 0; i < len4; i++) {
                    wages4[t][i] = 100.0;
                }
            }
            for (int t = 0; t < 2; t++) {
                for (int i = 0; i < len6; i++) {
                    wages6[t][i] = 100.0;
                }
            }
        
        }
        
        double get_value(Board2048 const& state) const {
            double value = 0.0;
            std::vector<Board2048> symmetries = get_symmetries(state);

            for (int t = 0; t < 2; ++t) {
                for (auto const& s : symmetries) {
                    int index = get_index(s, tuples[t]);
                    value += wages4[t][index];
                }
            }
            for (int t = 0; t < 2; ++t) {
                for (Board2048 const& s : symmetries) {
                    int index = get_index(s, tuples[2 + t]);
                    value += wages6[t][index];
                }
            }
            
            return value;
        }

        void learn(Board2048 const& state, double delta, double alpha) {
            std::vector<Board2048> symmetries = get_symmetries(state);
    
            // if (delta < 0)
            // std::cout << "learning : " << delta << "\n";

            for (int t = 0; t < 2; ++t) {
                for (Board2048 const& s : symmetries) {
                    int index = get_index(s, tuples[t]);
                    wages4[t][index] += delta * alpha;
                }
            }
            for (int t = 0; t < 2; ++t) {
                for (Board2048 const& s : symmetries) {
                    int index = get_index(s, tuples[2 + t]);
                    wages6[t][index] += delta * alpha;
                }
            }
        }

        void save(const char* fileName) {
            std::ofstream file(fileName);
            if (!file.is_open()) {
                // TODO: throw exception
            }

            for (int t = 0; t < 2; t++) {
                for (int i = 0; i < len4; i++) {
                    file << wages4[t][i] << " ";
                }
                file << "\n";
            }
            for (int t = 0; t < 2; t++) {
                for (int i = 0; i < len6; i++) {
                    file << wages6[t][i] << " ";
                }
                file << "\n";
            }
        
            file.close();
        }

};

#endif // NTUPLE2_H
