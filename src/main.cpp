#include <iostream>
#include "cmdparser.h"
#include "configuration.h"
#include "harmonic.h"

using namespace std;

void setup(CmdParser& parser) {
	parser.set_optional<string>("o", "output", "data.out", "Path where the measurements will be stored.");
	parser.set_optional<int>("n", "nt", 100, "The number of points in temporal direction.");
	parser.set_optional<double>("w", "omegasq", 1.0, "The value of omega², which is proportional to a.");
	parser.set_optional<double>("t", "tau", 1.0, "The precision of the integrator, since ε = τ / N_delta.");
	parser.set_optional<int>("d", "ndelta", 10, "The number of steps in the integrator, called N_delta.");
	parser.set_optional<int>("m", "nmeas", 1000, "The number of steps, i.e. the number of measurements.");
	parser.set_optional<int>("i", "ninit", 100, "The number of steps in the thermalization process.");
	parser.set_optional<int>("s", "seed", 0, "The seed for the random number generator.");
}

int main(int argc, char** argv) {
	CmdParser cmd { argc, argv };
	setup(cmd);
	cmd.parse_and_exit();

	const auto output = cmd.get<string>("o");
	const auto config = Configuration {
		cmd.get<int>("n"),
		cmd.get<double>("w"),
		cmd.get<int>("m"),
		cmd.get<int>("i"),
		cmd.get<double>("t"),
		cmd.get<int>("d"),
		cmd.get<int>("s")
	};

	cout << config << endl;
}
