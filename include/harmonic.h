#pragma once
#include "configuration.h"

class Harmonic final {
public:
	Harmonic(const Configuration& configuration);

	void run();

private:
	Configuration cfg;
};
