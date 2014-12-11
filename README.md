# Harmonic Oscillator

This is very simple HMC sample, to simulate the harmonic oscillator on the lattice. The code is written in C++11 and does not have any external dependencies. The lattice uses periodic boundary conditions.

Compiling the program requires a compiler such as gcc, clang, icc or MSVC in one of the more recent releases. A Jakefile has been included, which allows to build the sources using `jake release`, `jake debug` or just by using `jake`. Even though the sources do not have any dependencies, the **Jake** program is dependent on some other software. Here we require **nodejs** and **npm**.

The Jakefile will also use more advanced stuff, which makes the following setup efficient, if npm and nodejs are installed, but not Jake or local modules:

	npm install -g jake
	npm install
	jake release

The program can then be started using

	./bin/release/harmonic

By default output will be written in the file *data.out*.

Copyright (C) 2014 Florian Rappl

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License  along with this program.  If not, see <http://www.gnu.org/licenses/>.
