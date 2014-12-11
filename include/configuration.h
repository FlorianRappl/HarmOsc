#pragma once
#include <iostream>

struct Configuration {
public:
	int nt;
	double omega_square;
	int nmeas;
	int ntherm;
	double tau;
	int nstep;
	int seed;

	friend std::ostream& operator <<(std::ostream& os, const Configuration& config) {
		using std::endl;

		os << "Nt    = " << config.nt << endl;
		os << "ω²    = " << config.omega_square << endl;
		os << "Nmeas = " << config.nmeas << endl;
		os << "Nterm = " << config.ntherm << endl;
		os << "τ     = " << config.tau << endl;
		os << "Nstep = " << config.nstep << endl;
		os << "Seed  = " << config.seed;

		return os;
	}
};