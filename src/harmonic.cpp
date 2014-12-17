/*
  This file is part of the Harmonic Oscillator sample.
  Copyright (C) 2014 Florian Rappl
  
  This program is free software: you can redistribute it and/or
  modify it under the terms of the GNU General Public License as
  published by the Free Software Foundation, either version 3 of
  the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public
  License along with this program. If not, see
  http://www.gnu.org/licenses/.
*/

#include "harmonic.h"

physics::Harmonic::Harmonic(const physics::Configuration& cfg, std::ostream& info, std::ostream& warn) noexcept : 
	ntherm(cfg.ntherm),
	nmeas(cfg.nmeas),
	info(info),
	warn(warn),
	rng(cfg.seed),
	dist(0.0, 1.0),
	lattice(rng, cfg.nt, cfg.nstep, cfg.tau, cfg.omega_square, cfg.lambda),
	xsm(0.0),
	xsqm(0.0),
	acr(0.0) {
}

void physics::Harmonic::run(std::function<void(int, double, double, double)> report) noexcept {
	init();
	thermalize();
	measure(report);
}

void physics::Harmonic::init() noexcept {
	for (int n = 0; n < 10; ++n) {
		lattice.randomize();
		lattice.integrate();
	}
}

void physics::Harmonic::thermalize() noexcept {
	using std::endl;
	info << "Running thermalization ..." << endl;
	auto arate = 0.0;

	for (int n = 0; n < ntherm; ++n) {
		const auto accepted = step();
		const auto xs = lattice.x_average();
		const auto xsq = lattice.x_square_average();
		info << "Init-Update [" << n << "]" << endl;
		info << "acc  = " << accepted << endl;
		info << "<x>  = " << xs << endl;
		info << "<x²> = " << xsq << endl;
		arate += accepted;
	}

	info << "Thermalization finished!" << endl;

	if (4.0 * arate < ntherm) {
		warn << "Bad acceptance rate in thermalisation!" << endl;
		exit(1);
	}
}

void physics::Harmonic::measure(std::function<void(int, double, double, double)> report) noexcept {
	using std::endl;
	info << "Starting measurements ..." << endl;

	for (int n = 0; n < nmeas; ++n) { 
		const auto accepted = step();
		const auto xs = lattice.x_average();
		const auto xsq = lattice.x_square_average();
		const auto act = lattice.action_average();
		info << "Meas-Update [" << n << "]" << endl;
		info << "acc  = " << accepted << endl;
		info << "<x>  = " << xs << endl;
		info << "<x²> = " << xsq << endl;
		report(n, xs, xsq, act);
		acr += accepted;
		xsm += xs;
		xsqm += xsq;
	}

	info << "Measurements finished!" << endl;
}

bool physics::Harmonic::step() noexcept {
	lattice.randomize();
	lattice.store();
	const auto a = lattice.hamilton();
	lattice.integrate();
	const auto b = lattice.hamilton();
	const auto accept = metropolis(b - a);

	if (!accept)
		lattice.restore();

	return accept;
}

bool physics::Harmonic::metropolis(double r) noexcept {
	return r <= 0.0 || dist(rng) <= exp(-r);
}

double physics::Harmonic::compute_acceptance() const noexcept {
	return acr / static_cast<double>(nmeas);
}

double physics::Harmonic::compute_x() const noexcept {
	return xsm / static_cast<double>(nmeas);
}

double physics::Harmonic::compute_x_square() const noexcept {
	return xsqm / static_cast<double>(nmeas);
}
