#pragma once
#include <random>
#include "configuration.h"

class Lattice final {
public:
	explicit Lattice(const Configuration& cfg);

	~Lattice();

	double x(int index) const;

	double p(int index) const;

	void x(int index, double value);

	void p(int index, double value);

	void store();

	void restore();

	void randomize();

	void integrate();

	double hamilton() const;

	double x_average() const;

	double x_square_average() const;

	double action_average() const;

protected:
	double force(int n) const;

	void integrate_x(double eps);

	void integrate_p(double eps);

private:
	std::mt19937 rng;
	std::normal_distribution<double> gauss;
	int nt;
	int nstep;
	double osq;
	double eps;
	double* xv;
	double* xbck;
	double* pv;
};
