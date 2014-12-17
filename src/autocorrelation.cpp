#include "autocorrelation.h"
#include <cmath>
#include <limits>

using std::size_t;

int abs(int number) noexcept {
	return number >= 0 ? number : -number;
}

statistics::AutoCorrelation::AutoCorrelation(const std::vector<double>& elements, int lambda) noexcept :
	elements(elements),
	lambda(lambda),
	avg(average()) {
}

double statistics::AutoCorrelation::average() const noexcept {
	const auto n = elements.size();
	auto sum = 0.0;

	for (size_t i = 0; i < n; ++i)
		sum += elements[i];

	return sum / static_cast<double>(n);
}

statistics::Observable<double> statistics::AutoCorrelation::compute() const noexcept {
	using std::sqrt;

	const auto n = elements.size();
	const auto tmax = (n - lambda) / 2;
	double g[n];
	double eg[tmax];
	const auto g0 = auto_corr(n - 1, g);
	sigma_corr(tmax, g, eg);

	auto sigma = 0.0;
	auto tau = 0.5;
	auto w = tmax;

	if (g0 != 0.0) {
		for (size_t t = 1; t < w; ++t) {
			tau += g[t];

			if (g[t] <= eg[t]) {
				w = t;
				break;
			}
		}

		sigma = tau * sqrt((2.0 / static_cast<double>(n)) * ((2 * w + 1) - 3.0 * tau + 1.0 / (4.0 * tau)));
	}

	return statistics::Observable<double> { tau, sigma };
}

double statistics::AutoCorrelation::sigma() const noexcept {
	using std::sqrt;
	using std::pow;

	const auto n = elements.size();
	const auto xn = static_cast<double>(n);
	auto var = 0.0;

	for (size_t i = 0; i < n; ++i)
		var += pow(elements[i] - avg, 2);

	return sqrt(var / (xn * (xn - 1.0)));
}

double statistics::AutoCorrelation::auto_corr(int tmax, double* g) const noexcept {
	using std::abs;
	using std::numeric_limits;

	const auto n = elements.size();
	auto g0 = sigma();

	if (g0 <= (10.0 * numeric_limits<double>::epsilon() * abs(avg))) {
		g0 = 0.0;

		for (int t = 0; t < tmax; ++t)
			g[t] = 0.0;  
	} else {
		g0 = g0 * g0 * static_cast<double>(n - 1);  
		g[0] = 1.0;

		for (int t = 1; t < tmax; ++t) {
			auto var = 0.0;

			for (size_t k = 0; k < (n - t); ++k)
				var += ((elements[k] - avg) * (elements[k + t] - avg));

			g[t] = var / (g0 * static_cast<double>(n - t));
		}
	}

	return g0;
}

void statistics::AutoCorrelation::sigma_corr(int tmax, double *g, double *eg) const noexcept {
	using std::sqrt;
	using std::pow;

	const auto n = static_cast<double>(elements.size());
	eg[0] = 0.0;

	if (g[0] == 0.0) {
		for (int t = 1; t < tmax; ++t)
			eg[t] = 0.0;
	} else {
		for (int t = 1; t < tmax; ++t) {
			auto sm = 0.0;

			for (int k = 1; k <= (t + lambda); ++k)
				sm += pow(g[k + t] + g[abs(k - t)] - 2.0 * g[t] * g[k], 2);

			eg[t] = sqrt(sm / n);
		}
	}
}
