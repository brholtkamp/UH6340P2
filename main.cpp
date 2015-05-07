//
// Created by Brian on 4/15/2015.
//

#include <iostream>
#include <fstream>
#include <string>

#include "studentRecord.hpp"

#define DEBUG 0

std::string getNext(std::ifstream& file) {
	std::string buffer;

	if (file.good()) {
		std::getline(file, buffer);

		// See if it's a command
		if (buffer.find("*") != std::string::npos) {
#if DEBUG
			std::cout << buffer.substr(1) << std::endl;
#endif
			return buffer.substr(1);
		} else {
			// Drop it to lowercase since it's not a command
			for (auto& character : buffer) {
				character = static_cast<char>(tolower(character));
			}

#if DEBUG
			std::cout << buffer << std::endl;
#endif
			return buffer;
		}
	}

	return "";
}

int main() {
	studentRecord records;

	std::string filename;
	std::cout << "Please type in the filename for the input file: ";
	std::cin >> filename;

	commandType command = NONE;
	std::string buffer = "";
	std::string updateValue = "";

	std::ifstream file(filename);
	while (!file.is_open()) {
		std::cerr << "File not found, try another file name: ";
		std::cin >> filename;

		file.open(filename);
	}

	while (file.good()) {
		// Grab a piece of data
		buffer = getNext(file);
		updateValue = "";
		
		// Check to see if it's a command
		if (records.generateCommand(buffer) != NONE) {
			command = records.generateCommand(buffer);
			// See if it's a command that doesn't require an input
			if (command == SNAPSHOT || command == LIST) {
				records.handleCommand(command, buffer, updateValue);
				command = NONE;
			}
		} else {
			// Grab the next piece of data if it's an update
			if (command == UPDATE) {
				updateValue = getNext(file);
			}

			records.handleCommand(command, buffer, updateValue);
		}
	}

	std::cout << "Press enter to quit." << std::endl;
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cin.get();

    return 0;
}
