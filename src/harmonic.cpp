#include "harmonic.h"

Harmonic::Harmonic(const Configuration& cfg, std::ostream& info, std::ostream& warn) : 
	cfg(cfg),
	info(info),
	warn(warn),
	rng(cfg.seed),
	dist(0.0, 1.0),
	lattice(cfg),
	xsm(0.0),
	xsqm(0.0),
	acr(0.0) {
}

void Harmonic::run(std::ostream& data) {
	init();
	thermalize();
	measure(data);
}

void Harmonic::init() {
	for (int n = 0; n < 10; ++n) {
		lattice.randomize();
		lattice.integrate();
	}
}

void Harmonic::thermalize() {
	using std::endl;
	info << "Running thermalization ..." << endl;
	auto arate = 0.0;

	for (int n = 0; n < cfg.ntherm; ++n) {
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

	if (4.0 * arate < cfg.ntherm) {
		warn << "Bad acceptance rate in thermalisation!" << endl;
		exit(1);
	}
}

void Harmonic::measure(std::ostream& data) {
	using std::endl;
	info << "Starting measurements ..." << endl;

	for (int n = 0; n < cfg.nmeas; ++n) { 
		const auto accepted = step();
		const auto xs = lattice.x_average();
		const auto xsq = lattice.x_square_average();
		const auto act = lattice.action_average();
		info << "Meas-Update [" << n << "]" << endl;
		info << "acc  = " << accepted << endl;
		info << "<x>  = " << xs << endl;
		info << "<x²> = " << xsq << endl;
		data << n << "\t" << xs << "\t" << xsq << "\t" << act << endl;
		acr += accepted;
		xsm += xs;
		xsqm += xsq;
	}

	info << "Measurements finished!" << endl;
}

bool Harmonic::step() {
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

bool Harmonic::metropolis(double r) {
	return r <= 0.0 || dist(rng) <= exp(-r);
}

double Harmonic::compute_acceptance() const {
	return acr / static_cast<double>(cfg.nmeas);
}

double Harmonic::compute_x() const {
	return xsm / static_cast<double>(cfg.nmeas);
}

double Harmonic::compute_x_square() const {
	return xsqm / static_cast<double>(cfg.nmeas);
}
