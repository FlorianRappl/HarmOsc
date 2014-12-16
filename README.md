# Harmonic Oscillator

This is very simple HMC sample, to simulate the harmonic oscillator on the lattice. The code is written in C++11 and does not have any external dependencies. The lattice uses periodic boundary conditions.

## Building

Compiling the program requires a compiler such as GCC, Clang, ICC or MSVC in one of the more recent releases. A Jakefile has been included, which allows to build the sources using `jake release`, `jake debug` or just by using `jake`. Even though the sources do not have any dependencies, the **Jake** program is dependent on some other software. Here we require **nodejs** and **npm**.

The Jakefile will also use more advanced stuff, which makes the following setup efficient, if npm and nodejs are installed, but not Jake or local modules:

	npm install -g jake
	npm install
	jake release

The program can then be started using

	./bin/release/harmonic

By default output will be written in the file *data.out*.

## Output

The program produces a file that contains the complete history of the MC process. The file can be named via the command line arguments. By default the file is called *data.out*. Additionally to some debug information output, like the update process, a final resumee is printed. 

The final statistics may look as follows:

	acc  = 0.968
	<x>  = -0.00193317
	<x²> = 0.450043
	x²a  = 0.447214
	<τi> = 0.89825
	στi  = 0.103073

Here the first value is the acceptance rate (between 0 and 1). The second value is the average value of the sites, x. The same value is then printed with squared sites. This excludes any sign changes, resulting in a greater (absolute) value in general. The fourth value is the expected value from an analytic calculation. This value is suppossed to be `nan` for simulations with the anharmonic term (see next section). Finally the integrated auto-correlation time and its uncertainty are shown.

## Further information

The program follows the paper by Creutz, Freedman, which is available in Annals. of Physics 132 (1981), 427. However, the paper discusses the anharmonic oscillator, which is not exactly solvable. This is also the reason for using numerical methods. The anharmonic term is also part of the program and can be controlled with the lambda parameter (`-l`). The continuum action for the Euclidean time \tau is therefore given by

	S = \frac{1}{2} \int d\tau (\dot{x}²(\tau) + \omega² x²(\tau) + 2 \lambda x⁴(\tau)).

The discretization is

	S = \frac{1}{2} \sum_{n = 0}{N_t - 1} (2 x_{n}² - x_{n+1} x_{n} - x_{n-1} x_{n} + \omega² x_{n}² + 2 \lambda x_{n}⁴).

The force has been computed by taking the derivative of the action with respect to x_{i}. The derivative is straight forward, however, for a single term one needs to think of translation invariance.

## License

Copyright (C) 2014 Florian Rappl

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License  along with this program.  If not, see <http://www.gnu.org/licenses/>.
