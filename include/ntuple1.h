#ifndef NTUPLE1_H
#define NTUPLE1_H

#include "ntuple.h"

#include <fstream>
#include <iostream>

#include <vector>

typedef std::pair<int, int> pairii;
typedef std::vector<pairii> vecpii;

class NTuple1 : public NTuple {
	private:
	const static int numStates = 15;

	const static int len4 = 50625;
	const static int len6 = 11390625;

	std::vector<vecpii> tuples;
	
	long double wages[17][len4];
	
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
	NTuple1(char *fileName) {
		initTuples();

		std::ifstream file(fileName);
		if (!file.is_open()) {
			throw -1;
		}

		for (int t = 0; t < 17; t++) {
			for (int i = 0; i < len4; i++) {
				file >> wages[t][i];
			}
		}
		file.close();
	
	}

	NTuple1() : tuples() {
		initTuples();
		for (int t = 0; t < 17; t++) {
			for (int i = 0; i < len4; i++) {
				wages[t][i] = 0.0;
			}
		}

	}
	
	double get_value(Board2048 const& state) const {
		double value = 0.;

		for (int t = 0; t < 17; ++t) {
			int index = get_index(state, tuples[t]);
			value += wages[t][index];
		}
		
		return value;
	}

	void learn(Board2048 const& state, double delta, double alpha) {
		for (int t = 0; t < 17; ++t) {
			int index = get_index(state, tuples[t]);
			wages[t][index] += alpha * delta;
		}
	}

	void save(const char* fileName) {
		std::ofstream file(fileName);
		if (!file.is_open()) {
			// TODO: throw exception
		}

		for (int t = 0; t < 17; t++) {
			for (int i = 0; i < len4; i++) {
				file << wages[t][i] << " ";
			}
			file << "\n";
		}

		file.close();
	}

};

#endif // NTUPLE1_H
