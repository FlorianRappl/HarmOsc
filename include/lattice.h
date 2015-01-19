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
	/**
	* Lattice management class.
	*/
	class Lattice final {
	public:
		/**
		* Constructs a new Lattice object.
		*
		* @param The reference to the random number generator to use.
		* @param The number of temporal sites.
		* @param The number of integration steps.
		* @param The integration trajectory length.
		* @param The harmonic parameter, ω².
		* @param The anharmonic parameter, λ.
		*/
		Lattice(std::mt19937& rng, int nt, int nstep, double tau, double omegasq, double lambda) noexcept;

		/**
		* Cleans everything up.
		*/
		~Lattice() noexcept;

		/**
		* Gets the value at the specified site.
		* 
		* @param The index of the site.
		* @return The value of x_i.
		*/
		double x(int index) const noexcept;

		/**
		* Gets the momentum at the specified site.
		* 
		* @param The index of the site.
		* @return The value of p_i.
		*/
		double p(int index) const noexcept;

		/**
		* Sets the value at the specified site.
		* 
		* @param The index of the site.
		* @param The new value of x_i.
		*/
		void x(int index, double value) noexcept;

		/**
		* Sets the momentum at the specified site.
		* 
		* @param The index of the site.
		* @param The new value of p_i.
		*/
		void p(int index, double value) noexcept;

		/**
		* Stores the current state of the lattice.
		*/
		void store() noexcept;

		/**
		* Restores the state of the lattice to the previously saved state.
		*/
		void restore() noexcept;

		/**
		* Randomizes the momenta.
		*/
		void randomize() noexcept;

		/**
		* Integrates the sites' values by using their momenta.
		*/
		void integrate() noexcept;

		/**
		* Computes the value of the Hamilton operator.
		* 
		* @return The value of H, which is p²/2m + L.
		*/
		double hamilton() const noexcept;

		/**
		* Gets the average value of the sites.
		* 
		* @return The value averaged over all sites.
		*/
		double x_average() const noexcept;

		/**
		* Gets the average squared value of the sites.
		* 
		* @return The value over all sites.
		*/
		double x_square_average() const noexcept;

		/**
		* Gets the average value of the action over all sites.
		* 
		* @return The average action value.
		*/
		double action_average() const noexcept;

	protected:
		/**
		* Calculates the force at a specific site.
		* 
		* @param The index of the site.
		* @return The value of the force at the given site.
		*/
		double force(int n) const noexcept;

		/**
		* Performs a single integration step over all sites.
		* 
		* @param The step size.
		*/
		void integrate_x(double eps) noexcept;

		/**
		* Performs a single integration step over all momenta.
		* 
		* @param The step size.
		*/
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
