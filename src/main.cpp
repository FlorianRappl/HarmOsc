#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include "cmdparser.h"
#include "configuration.h"
#include "harmonic.h"
#include "autocorrelation.h"

using namespace std;

void setup(CmdParser& parser) {
	parser.set_optional<string>("o", "output", "data.out", "Name of the file that will be used for output.");
	parser.set_optional<int>("n", "nt", 100, "The number of points in temporal direction.");
	parser.set_optional<double>("w", "omegasq", 1.0, "The value of the coupling ω². It is ω ~ a.");
	parser.set_optional<double>("t", "tau", 1.0, "The trajectory length in molecular dynamics time, where ε = τ / nsteps.");
	parser.set_optional<int>("l", "nsteps", 10, "The number of leapfrog steps per trajectory.");
	parser.set_optional<int>("m", "nmeas", 1000, "The number of steps, i.e. the number of measurements.");
	parser.set_optional<int>("i", "ninit", 100, "The number of steps in the thermalization process.");
	parser.set_optional<int>("s", "seed", 0, "The seed for the random number generator.");
	parser.set_optional<bool>("h", "noconsole", false, "Deactivates terminal output during measurements.");
}

void parse_and_exit(CmdParser& parser) {
	if (parser.parse() == false)
		exit(1);
}

double compute_analytic(const Configuration& cfg) {
	using std::sqrt;
	using std::pow;

	const auto nt = cfg.nt;
	const auto omegasq = cfg.omega_square;
	const auto omega = sqrt(omegasq);

	const auto c = omega * sqrt(1.0 + omegasq / 4.0);
	const auto r = 1.0 + 0.5 * omegasq - c;
	const auto a = (1.0 + pow(r, nt)) / (1.0 - pow(r, nt));
	const auto b = 2.0 * c;
	return a / b;
}

int main(int argc, char** argv) {
	vector<double> xsquares { };
	stringstream ss { };
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
		cmd.get<int>("l"),
		cmd.get<int>("s")
	};

	cout << config << endl;

	Harmonic sim { 
		config, 
		cmd.get<bool>("h") ? ss : cout, 
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
	cout << "x²a  = " << compute_analytic(config) << endl;
	cout << "<τi> = " << tauInt.mean << endl;
	cout << "στi  = " << tauInt.uncertainty << endl;
}
