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

namespace physics {
	/**
	* DTO structure to carry basic configuration information.
	*/
	struct Configuration {
	public:
		/**
		* The number of sites.
		*/
		int nt;
		/**
		* The square of the resonance frequency.
		*/
		double omega_square;
		/**
		* The anharmonic coupling.
		*/
		double lambda;
		/**
		* The number of measurements.
		*/
		int nmeas;
		/**
		* The number of thermalization steps.
		*/
		int ntherm;
		/**
		* The integration length.
		*/
		double tau;
		/**
		* The number of integration steps.
		*/
		int nstep;
		/**
		* The seed for the random number generator.
		*/
		int seed;

		friend std::ostream& operator <<(std::ostream& os, const Configuration& config) {
			using std::endl;

			os << "Nt    = " << config.nt << endl;
			os << "ω²    = " << config.omega_square << endl;
			os << "λ     = " << config.lambda << endl;
			os << "Nmeas = " << config.nmeas << endl;
			os << "Nterm = " << config.ntherm << endl;
			os << "τ     = " << config.tau << endl;
			os << "Nstep = " << config.nstep << endl;
			os << "Seed  = " << config.seed;

			return os;
		}
	};
}
