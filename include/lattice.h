#pragma once
#include <random>

namespace physics {
	class Lattice final {
	public:
		explicit Lattice(std::mt19937& rng, int nt, int nstep, double tau, double omegasq, double lambda) noexcept;

		~Lattice() noexcept;

		double x(int index) const noexcept;

		double p(int index) const noexcept;

		void x(int index, double value) noexcept;

		void p(int index, double value) noexcept;

		void store() noexcept;

		void restore() noexcept;

		void randomize() noexcept;

		void integrate() noexcept;

		double hamilton() const noexcept;

		double x_average() const noexcept;

		double x_square_average() const noexcept;

		double action_average() const noexcept;

	protected:
		double force(int n) const noexcept;

		void integrate_x(double eps) noexcept;

		void integrate_p(double eps) noexcept;

	private:
		std::mt19937& rng;
		std::normal_distribution<double> gauss;
		int nt;
		int nstep;
		double osq;
		double lambda;
		double eps;
		double* xv;
		double* xbck;
		double* pv;
	};
}
