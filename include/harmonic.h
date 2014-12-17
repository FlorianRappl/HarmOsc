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
#include <iostream>
#include <functional>
#include "configuration.h"
#include "lattice.h"

namespace physics {
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
}
