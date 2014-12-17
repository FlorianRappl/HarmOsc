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
	template<class T>
	struct Observable {
		T mean;
		T uncertainty;
	};

	class AutoCorrelation final {
	public:
		AutoCorrelation(const std::vector<double>& elements, int lambda = 100) noexcept;

		Observable<double> compute() const noexcept;

	protected:
		double average() const noexcept;

		double sigma() const noexcept;

		void sigma_corr(int tmax, double *g, double *eg) const noexcept;

		double auto_corr(int tmax, double* g) const noexcept;

	private:
		std::vector<double> elements;
		int lambda;
		double avg;
	};
}
