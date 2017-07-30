#ifndef NTUPLE_H
#define NTUPLE_H

#include "board.h"

class NTuple {
	public :
	    virtual double get_value(Board2048 const& state) const = 0;
	    virtual void learn(Board2048 const& state, double delta, double alpha) = 0;
	    virtual void save(const char* fileName) = 0;

	    virtual ~NTuple() {};
};

#endif // NTUPLE_H
