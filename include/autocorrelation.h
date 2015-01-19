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
#include <vector>

namespace statistics {
	/**
	* An observable quantity that has a measured average and uncertainty.
	*/
	template<class T>
	struct Observable {
	public:
		T mean;
		T uncertainty;
	};

	/**
	* Statistic helper to compute the autocorrelation of a set of elements.
	*/
	class AutoCorrelation final {
	public:
		/**
		* Constructs a new Autocorrelation helper.
		*
		* @param The elements to consider.
		* @param The value of the lambda parameter to control the offset.
		*/
		AutoCorrelation(const std::vector<double>& elements, int lambda = 100) noexcept;

		/**
		* Computes an observable that contains the statistics information.
		*
		* @return A structure consisting of mean and uncertainty information.
		*/
		Observable<double> compute() const noexcept;

	protected:
		/**
		* Computes the average over all elements.
		*
		* @return A number representing the average.
		*/
		double average() const noexcept;

		/**
		* Computes the standard deviation over all elements.
		*
		* @return A number representing the std. deviation.
		*/
		double sigma() const noexcept;

		/**
		* Computes the standard deviation values for the autocorrelation values.
		*
		* @param The number of elements to consider at maximum.
		* @param The autocorrelation values that should be used.
		* @param The target where the respective standard deviations are stored.
		*/
		void sigma_corr(int tmax, double* source, double* result) const noexcept;

		/**
		* Computes the auto correlation for the sliding window.
		*
		* @param The number of elements to consider at maximum.
		* @param The target where all autocorrelation values are stored.
		* @return The integrated autocorrelation value.
		*/
		double auto_corr(int tmax, double* result) const noexcept;

	private:
		std::vector<double> elements;
		int lambda;
		double avg;
	};
}
