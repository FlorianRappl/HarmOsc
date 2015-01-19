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
	/**
	* Class that drives the (an-)harmonic oscillator simulation.
	*/
	class Harmonic final {
	public:
		/**
		* Constructs a new Harmonic simulation object.
		*
		* @param The configuration to use for the simulation.
		* @param The stream to write information to.
		* @param The stream to write errors to.
		*/
		Harmonic(const Configuration& configuration, std::ostream& info, std::ostream& warn) noexcept;

		/**
		* Runs a simulation with all previously defined parameters.
		*
		* @param The callback to report progress to.
		*/
		void run(std::function<void(int, double, double, double)> report) noexcept;

		/**
		* Gets the current acceptance rate.
		*
		* @return The current acceptance rate.
		*/
		double compute_acceptance() const noexcept;

		/**
		* Gets the current average x values.
		*
		* @return The average x values.
		*/
		double compute_x() const noexcept;

		/**
		* Gets the current average squared x values.
		*
		* @return The average squared x values.
		*/
		double compute_x_square() const noexcept;

	protected:
		/**
		* Determines if the given delta should be accepted.
		*
		* @param The delta of the action (new - old).
		* @return True if the change should be accepted, otherwise false.
		*/
		bool metropolis(double r) noexcept;

		/**
		* Runs a single update step.
		*
		* @return True if the change has been accepted, otherwise false.
		*/
		bool step() noexcept;

		/**
		* Runs the initialization process.
		*/
		void init() noexcept;

		/**
		* Runs the thermalization process.
		*/
		void thermalize() noexcept;

		/**
		* Runs the measurement process, which also reports statistics.
		*
		* @param The callback to report progress to.
		*/
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
