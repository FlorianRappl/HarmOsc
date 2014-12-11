#pragma once
#include <vector>

template<class T>
struct Observable {
	T mean;
	T uncertainty;
};

class AutoCorrelation final {
public:
	AutoCorrelation(const std::vector<double>& elements, int lambda = 100);

	Observable<double> compute() const;

protected:
	double average() const;

	double sigma0() const;

	void sigma_corr(int tmax, double *g, double *eg) const;

	double auto_corr(int tmax, double* g) const;

private:
	std::vector<double> elements;
	int lambda;
	double avg;
};
