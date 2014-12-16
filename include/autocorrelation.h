#pragma once
#include <vector>

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
