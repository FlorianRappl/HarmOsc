#include "lattice.h"
#include <cmath>

inline int periodic(int index, int volume) noexcept {
	return index < 0 ? (index + volume) : (index >= volume ? index - volume : index);
}

Lattice::Lattice(const Configuration& cfg) noexcept :
	rng(cfg.seed),
	gauss(),
	nt(cfg.nt),
	nstep(cfg.nstep),
	osq(2.0 + cfg.omega_square),
	lambda(cfg.lambda),
	eps(cfg.tau / static_cast<double>(cfg.nstep)),
	xv(new double[cfg.nt]),
	xbck(new double[cfg.nt]),
	pv(new double[cfg.nt]) {
	const auto factor = 1.0 / sqrt(2.0 * cfg.omega_square);

	for(int i = 0; i < cfg.nt; ++i)
		xv[i] = gauss(rng) * factor;
}

Lattice::~Lattice() noexcept {
	delete[] xv;
	delete[] xbck;
	delete[] pv;
}

double Lattice::x(int index) const noexcept {
	return xv[periodic(index, nt)];
}

double Lattice::p(int index) const noexcept {
	return pv[periodic(index, nt)];
}

void Lattice::x(int index, double value) noexcept {
	xv[periodic(index, nt)] = value;
}

void Lattice::p(int index, double value) noexcept {
	pv[periodic(index, nt)] = value;
}

void Lattice::store() noexcept {
	for (int i = 0; i < nt; ++i)
		xbck[i] = xv[i];
}

void Lattice::restore() noexcept {
	for (int i = 0; i < nt; ++i)
		xv[i] = xbck[i];
}

void Lattice::randomize() noexcept {
	for (int i = 0; i < nt; ++i)
		pv[i] = gauss(rng);
}

double Lattice::force(int n) const noexcept {
	return osq * x(n) - x(n - 1) - x(n + 1) + 4.0 * lambda * pow(x(n), 3);
}

void Lattice::integrate_x(double eps) noexcept {
	for (int i = 0; i < nt; ++i)
		xv[i] += eps * pv[i];
}

void Lattice::integrate_p(double eps) noexcept {
	for (int i = 0; i < nt; ++i)
		pv[i] -= eps * force(i);
}

void Lattice::integrate() noexcept {
	integrate_x(eps * 0.5);
	integrate_p(eps);

	for (int i = 1; i < nstep; ++i) {
		integrate_x(eps);
		integrate_p(eps);
	}

	integrate_x(eps * 0.5);
}

double Lattice::hamilton() const noexcept {
	using std::pow;
	auto sum = 0.0;

	for(int i = 0; i < nt; ++i) {
		sum += pow(p(i), 2);
		sum += osq * pow(x(i), 2) + 2.0 * lambda * pow(x(i), 4);
		sum -= x(i) * (x(i - 1) + x(i + 1));
	}

	return 0.5 * sum;
}

double Lattice::x_average() const noexcept {
	auto sum = 0.0;

	for (int i = 0; i < nt; ++i)
		sum += xv[i];

	return sum / static_cast<double>(nt);
}

double Lattice::x_square_average() const noexcept {
	auto sum = 0.0;

	for (int i = 0; i < nt; ++i)
		sum += xv[i] * xv[i];

	return sum / static_cast<double>(nt);
}

double Lattice::action_average() const noexcept {
	using std::pow;
	auto sum = 0.0;

	for (int i = 0; i < nt; ++i) {
		sum += osq * pow(x(i), 2) + 2.0 * lambda * pow(x(i), 4);
		sum -= x(i) * (x(i - 1) + x(i + 1));
	}

	return 0.5 * sum / static_cast<double>(nt);
}
