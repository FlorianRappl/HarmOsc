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
