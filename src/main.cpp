#include <iostream>
#include <fstream>
#include <vector>
#include "cmdparser.h"
#include "configuration.h"
#include "harmonic.h"
#include "autocorrelation.h"

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

void parse_and_exit(CmdParser& parser) {
	if (parser.parse() == false)
		exit(1);
}

int main(int argc, char** argv) {
	vector<double> xsquares { };
	CmdParser cmd { 
		argc, 
		argv 
	};

	setup(cmd);
	parse_and_exit(cmd);

	ofstream output { 
		cmd.get<string>("o") 
	};

	Configuration config {
		cmd.get<int>("n"),
		cmd.get<double>("w"),
		cmd.get<int>("m"),
		cmd.get<int>("i"),
		cmd.get<double>("t"),
		cmd.get<int>("d"),
		cmd.get<int>("s")
	};

	cout << config << endl;

	Harmonic sim { 
		config, 
		cout, 
		cerr 
	};

	sim.run([&output, &xsquares](int n, double x, double xsquare, double action) {
		output << n << "\t" << x << "\t" << xsquare << "\t" << action << endl;
		xsquares.push_back(xsquare);
	});
	output.close();

	const auto tauInt = AutoCorrelation(xsquares).compute();

	cout << "Measurements statistics ..." << endl;
	cout << "acc  = " << sim.compute_acceptance() << endl;
	cout << "<x>  = " << sim.compute_x() << endl;
	cout << "<x²> = " << sim.compute_x_square() << endl;
	cout << "<τi> = " << tauInt.mean << endl;
	cout << "στi  = " << tauInt.uncertainty << endl;
}
