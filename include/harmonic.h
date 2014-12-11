#pragma once
#include <iostream>
#include <functional>
#include "configuration.h"
#include "lattice.h"

class Harmonic final {
public:
	Harmonic(const Configuration& configuration, std::ostream& info, std::ostream& warn);

	void run(std::function<void(int, double, double, double)> report);

	double compute_acceptance() const;

	double compute_x() const;

	double compute_x_square() const;

protected:
	bool metropolis(double r);

	bool step();

	void init();

	void thermalize();

	void measure(std::function<void(int, double, double, double)> report);

	inline double x(int n) const {
		return lattice.x(n);
	}

	inline double p(int n) const {
		return lattice.p(n);
	}

private:
	Configuration cfg;
	std::ostream& info;
	std::ostream& warn;
	std::mt19937 rng;
	std::uniform_real_distribution<double> dist;
	Lattice lattice;
	double xsm;
	double xsqm;
	double acr;
};
