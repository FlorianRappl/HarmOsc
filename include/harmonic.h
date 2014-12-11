#pragma once
#include <iostream>
#include "configuration.h"

class Harmonic final {
public:
	Harmonic(const Configuration& configuration, std::ostream& info, std::ostream& data, std::ostream& warn);

	void run();

private:
	Configuration cfg;
	std::ostream& info;
	std::ostream& data;
	std::ostream& warn;
};
