#pragma once
#include <iostream>
#include <functional>
#include "configuration.h"
#include "lattice.h"

class Harmonic final {
public:
	Harmonic(const Configuration& configuration, std::ostream& info, std::ostream& warn) noexcept;

	void run(std::function<void(int, double, double, double)> report) noexcept;

	double compute_acceptance() const noexcept;

	double compute_x() const noexcept;

	double compute_x_square() const noexcept;

protected:
	bool metropolis(double r) noexcept;

	bool step() noexcept;

	void init() noexcept;

	void thermalize() noexcept;

	void measure(std::function<void(int, double, double, double)> report) noexcept;

private:
	int ntherm;
	int nmeas;
	std::ostream& info;
	std::ostream& warn;
	std::mt19937 rng;
	std::uniform_real_distribution<double> dist;
	Lattice lattice;
	double xsm;
	double xsqm;
	double acr;
};
