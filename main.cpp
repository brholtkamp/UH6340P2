//
// Created by Brian on 4/15/2015.
//

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

#include "Tree/tree.hpp"

int main() {
    tree<std::string, std::string> tree;

	std::string filename;
	std::cout << "Please type in the filename for the input file: ";
	std::cin >> filename;

	std::ifstream file(filename);
	std::string buffer;
	std::string currentCommand = "";
	while (file.good()) {
		std::getline(file, buffer);

		// See if this is a command
		if (buffer.find("*") != std::string::npos) {
			currentCommand = buffer.substr(1, buffer.size());
		} else {
			std::string name = buffer;
			for (auto &character : name) {
				character = static_cast<char>(tolower(character));
			}

			if (currentCommand == "INSERT") {
				if (!tree.insert(name, "")) {
					std::cerr << "Failed to insert: " << name << std::endl;
				} else {
					// Success!
				}
			} else if (currentCommand == "UPDATE") {
				std::string note;
				std::getline(file, note);
				if (!tree.update(name, note)) {
					std::cerr << "Failed to update: " << name << " with " << note << std::endl;
				} else {
					// Success!
				}
			} else if (currentCommand == "SEARCH") {
				std::shared_ptr<std::string> result = tree.search(name);
				if (result == nullptr) {
					std::cerr << "Failed to find: " << name << std::endl;
				} else {
					// Success!
					std::cout << *(result) << std::endl;
				}
			} else if (currentCommand == "DELETE") {
				if (!tree.remove(name)) {
					std::cerr << "Failed to remove: " << name << std::endl;
				} else {
					// Success!
				}
			} else if (currentCommand == "SNAPSHOT") {
				tree.list();
			}
		}
	}

	std::cout << "Press enter to quit." << std::endl;
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cin.get();

    return 0;
}
