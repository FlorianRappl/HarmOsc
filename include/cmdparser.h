/*
  This file is part of the Harmonic Oscillator sample.
  Copyright (C) 2014 Florian Rappl
  
  This program is free software: you can redistribute it and/or
  modify it under the terms of the GNU General Public License as
  published by the Free Software Foundation, either version 3 of
  the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public
  License along with this program. If not, see
  http://www.gnu.org/licenses/.
*/

#pragma once
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <sstream>
#include <functional>

class CmdParser {
private:
	class CmdBase {
	public:
		CmdBase(const std::string& name, const std::string& alternative, const std::string& description, bool required) : 
			name(name),
			command(name.size() > 0 ? "-" + name : ""),
			alternative(alternative.size() > 0 ? "--" + alternative : ""),
			description(description),
			required(required),
			handled(false),
			arguments({}) {
		}

		virtual ~CmdBase() {
		}

		std::string name;
		std::string command;
		std::string alternative;
		std::string description;
		bool required;
		bool handled;
		std::vector<std::string> arguments;

		virtual std::string print_value() const = 0;
		virtual bool parse() = 0;

		bool is(const std::string& given) const {
			return given == command || given == alternative;
		}
	};

	template<typename T>
	class CmdArgument;

	static int parse(const CmdArgument<int>& source) {
		if (source.arguments.size() != 1)
			throw std::bad_cast();

		return std::stoi(source.arguments[0]);
	}

	static bool parse(const CmdArgument<bool>& source) {
		if (source.arguments.size() != 0)
			throw std::runtime_error("A boolean command line parameter cannot have any arguments.");

		return !source.value;
	}

	static double parse(const CmdArgument<double>& source) {
		if (source.arguments.size() != 1)
			throw std::bad_cast();

		return std::stod(source.arguments[0]);
	}

	static float parse(const CmdArgument<float>& source) {
		if (source.arguments.size() != 1)
			throw std::bad_cast();

		return std::stof(source.arguments[0]);
	}

	static long double parse(const CmdArgument<long double>& source) {
		if (source.arguments.size() != 1)
			throw std::bad_cast();

		return std::stold(source.arguments[0]);
	}

	static unsigned int parse(const CmdArgument<unsigned int>& source) {
		if (source.arguments.size() != 1)
			throw std::bad_cast();

		return static_cast<unsigned int>(std::stoul(source.arguments[0]));
	}

	static unsigned long parse(const CmdArgument<unsigned long>& source) {
		if (source.arguments.size() != 1)
			throw std::bad_cast();

		return std::stoul(source.arguments[0]);
	}

	static long parse(const CmdArgument<long>& source) {
		if (source.arguments.size() != 1)
			throw std::bad_cast();

		return std::stol(source.arguments[0]);
	}

	static std::string parse(const CmdArgument<std::string>& source) {
		if (source.arguments.size() != 1)
			throw std::bad_cast();

		return source.arguments[0];
	}

	template<typename T>
	static std::vector<T> parse(const CmdArgument<std::vector<T>>& source) {
		std::vector<T> values { };
		CmdArgument<T> buffer { source.name, source.name, "", false };
		buffer.arguments.resize(1);

		for (const auto& element : source.arguments) {
			buffer.arguments[0] = element;
			const auto value = parse(buffer);
			values.push_back(value);
		}

		return values;
	}

	template<typename T>
	static std::string stringify(const T& value) {
		return std::to_string(value);
	}

	static std::string stringify(const std::string& str) {
		return str;
	}

	template<typename T>
	class CmdArgument final : public CmdBase {
	public:
		T value;

		CmdArgument(const std::string& name, const std::string& alternative, const std::string& description, bool required) : 
			CmdBase(name, alternative, description, required) {
		}

		virtual bool parse() {
			try {
				value = CmdParser::parse(*this);
				return true;
			} catch (...) {
				return false;
			}
		}

		virtual std::string print_value() const {
			return stringify(value);
		}
	};

	const std::string _appname;
	std::vector<std::string> _arguments;
	std::vector<CmdBase*> _commands;

	CmdBase* find(const std::string& name) {
		for (auto command : _commands) {
			if (command->is(name))
				return command;
		}

		return nullptr;
	}

	CmdBase* find_default() {
		for (auto command : _commands) {
			if (command->name == "")
				return command;
		}

		return nullptr;
	}

	std::string usage() const {
		std::stringstream ss { };
		ss << "Available parameters:" << std::endl << std::endl;

		for (const auto command : _commands) {
			ss << "  " << command->command << "\t" << command->alternative; 

			if (command->required == true)
				ss << "\t(required)";

			ss << std::endl << "   " << command->description;

			if (command->required == false)
				ss << std::endl << "   " << "This parameter is optional. The default value is '" + command->print_value() << "'.";

			ss << std::endl << std::endl;
		}

		return ss.str();
	}

	std::string howto_required(CmdBase* command) const {
		std::stringstream ss { };
		ss << "The parameter " << command->name << " is required." << std::endl;
		ss << command->description << std::endl;
		ss << "For more help use --help or -h." << std::endl;
		return ss.str();
	}

	std::string howto_use(CmdBase* command) const {
		std::stringstream ss { };
		ss << "The parameter " << command->name << " has invalid arguments." << std::endl;
		ss << command->description << std::endl;
		ss << "For more help use --help or -h." << std::endl;
		return ss.str();
	}

	std::string no_default() const {
		std::stringstream ss { };
		ss << "No default parameter has been specified." << std::endl;
		ss << "The given argument must be used with a parameter." << std::endl;
		ss << "For more help use --help or -h." << std::endl;
		return ss.str();
	}

public:
	CmdParser(int argc, const char** argv) : _appname(argv[0]) {
		for (int i = 1; i < argc; ++i) {
			_arguments.push_back(argv[i]);
		}
	}

	CmdParser(int argc, char** argv) : _appname(argv[0]) {
		for (int i = 1; i < argc; ++i) {
			_arguments.push_back(argv[i]);
		}
	}

	~CmdParser() {
		for (int i = 0, n = _commands.size(); i < n; ++i) {
			delete _commands[i];
		}
	}

	template<typename T>
	void set_default(bool is_required, const std::string& description = "") {
		auto command = new CmdArgument<T> { "", "", description, is_required };
		_commands.push_back(command);
	}

	template<typename T>
	void set_required(const std::string& name, const std::string& alternative, const std::string& description = "") {
		auto command = new CmdArgument<T> { name, alternative, description, true };
		_commands.push_back(command);
	}

	template<typename T>
	void set_optional(const std::string& name, const std::string& alternative, T defaultValue, const std::string& description = "") {
		auto command = new CmdArgument<T> { name, alternative, description, false };
		command->value = defaultValue;
		_commands.push_back(command);
	}

	bool parse(bool displayOutput = true) {
		if (_arguments.size() > 0) {
			if (_arguments[0] == "--help" || _arguments[0] == "-h") {
				if (displayOutput)
					std::cout << usage();

				return false;
			}

			CmdBase* current = find_default();

			for (int i = 0, n = _arguments.size(); i < n; ++i) {
				auto associated = (_arguments[i].size() > 0 && _arguments[i][0] == '-') ? find(_arguments[i]) : nullptr;

				if (associated != nullptr) {
					current = associated;
					associated->handled = true;
				} else if (current == nullptr) {
					if (displayOutput)
						std::cerr << no_default();

					return false;
				} else
					current->arguments.push_back(_arguments[i]);
			}
		}

		for (auto command : _commands) {
			if (command->required && !command->handled) {
				if (displayOutput)
					std::cerr << howto_required(command);

				return false;
			}
		}

		for (auto command : _commands) {
			if (command->handled && !command->parse()) {
				if (displayOutput)
					std::cerr << howto_use(command);

				return false;
			}
		}

		return true;
	}

	template<typename T>
	T get(const std::string& name) const {
		for (const auto command : _commands) {
			if (command->name == name) {
				auto cmd = dynamic_cast<CmdArgument<T>*>(command);

				if (cmd == nullptr)
					throw std::runtime_error("Invalid usage of the parameter " + name + " detected.");

				return cmd->value;
			}
		}

		throw std::runtime_error("The parameter " + name + " could not be found.");
	}

	template<typename T>
	T get_if(const std::string& name, std::function<T(T)> callback) const {
		auto value = get<T>(name);
		return callback(value);
	}

	int minParameters() const {
		int count = 0;

		for (const auto command : _commands) {
			if (command->required)
				++count;
		}

		return count;
	}

	int maxParameters() const {
		return static_cast<int>(_commands.size());
	}

	const std::string& app_name() const {
		return _appname;
	}
};
