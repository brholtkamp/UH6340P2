//
// Created by Brian on 5/6/2015.
//

#ifndef PROJECT_2_STUDENT_RECORD_HPP
#define PROJECT_2_STUDENT_RECORD_HPP

#define FILEOUTPUT 1

#include <iostream>
#include <array>
#include <string>
#include <sstream>
#include <algorithm>

#include "Tree/tree.hpp"

const unsigned int BLOCK_SIZE = 4;
const unsigned int NAME_SIZE = 32;
const unsigned int INFORMATION_SIZE = 224;
const unsigned int STUDENT_SIZE = NAME_SIZE + INFORMATION_SIZE;

#if FILEOUTPUT
std::ofstream output;
#endif

class block {
public:
	inline std::string getStudentName(unsigned int index) {
		std::stringstream buffer;
		
		for (unsigned int i = index * STUDENT_SIZE; i < (index * STUDENT_SIZE) + NAME_SIZE; i++) {
			buffer << data[i];
		}

		return buffer.str();
	}

	inline std::string getStudentInformation(unsigned int index) {
		std::stringstream buffer;
		
		for (unsigned int i = (index * STUDENT_SIZE) + NAME_SIZE; i < (index + 1) * STUDENT_SIZE; i++) {
			buffer << data[i];
		}

		return buffer.str();
	}

	inline void setStudentName(unsigned int index, std::string input) { 
		unsigned int i;
		if (input.size() <= NAME_SIZE) {
			for (i = 0; i < input.size() + 1; i++) {
				data[(index * STUDENT_SIZE) + i] = input[i];
			}
		}
		else {
			for (i = 0; i < NAME_SIZE - 1; i++) {
				data[(index * STUDENT_SIZE) + i] = input[i];
			}
		}
		data[(index * STUDENT_SIZE) + i] = '\0';
	}

	inline void setStudentInformation(unsigned int index, std::string input) {
		unsigned int i;
		if (input.size() <= INFORMATION_SIZE) {
			for (i = 0; i < input.size() + 1; i++) {
				data[(index * STUDENT_SIZE) + NAME_SIZE + i] = input[i];
			}
		}
		else {
			for (i = 0; i < INFORMATION_SIZE - 1; i++) {
				data[(index * STUDENT_SIZE) + NAME_SIZE + i] = input[i];
			}
		}
		data[(index * STUDENT_SIZE + NAME_SIZE) + i] = '\0';
	}

	inline void clearStudentName(unsigned int index) {
		for (unsigned int i = 0; i < NAME_SIZE; i++) {
			data[(index * STUDENT_SIZE) + i] = '\0';
		}
	}

	inline void clearStudentInformation(unsigned int index) {
		for (unsigned int i = 0; i < INFORMATION_SIZE; i++) {
			data[(index * STUDENT_SIZE) + NAME_SIZE + i] = '\0';
		}
	}

	inline bool studentNameExists(unsigned int index) {
		return data[(index * STUDENT_SIZE)] == '\0';
	}

	inline bool studentInformationExists(unsigned int index) {
		return data[(index * STUDENT_SIZE) + NAME_SIZE] == '\0';
	}

	inline bool blockEmpty() {
		bool result = true;
		for (unsigned int i = 0; i < BLOCK_SIZE; i++) {
			if (studentNameExists(i) || studentInformationExists(i)) {
				result = false;
			}
		}

		return result;
	}
	
	std::array<char, 1024> data;
};

enum commandType {
	INSERT,
	UPDATE,
	SEARCH,
	DELETE,
	LIST,
	SNAPSHOT,
	NONE
};

class studentRecord {
public:
	studentRecord() : tree(), blocks(), currentBlock(nullptr), currentStudent(0), currentCommand(SNAPSHOT) { 
#if FILEOUTPUT
		output.open("out.txt");
#endif
	}

	commandType generateCommand(const std::string input);
	bool handleCommand(const commandType command, const std::string key, const std::string value);

	~studentRecord() {
#if FILEOUTPUT
		output.close();
#endif
	}

protected:
	bool insert(const std::string key);
	bool update(const std::string key, const std::string value);
	bool search(const std::string key);
	bool remove(const std::string key);
	void list();
	void snapshot();

private:
	std::vector<std::shared_ptr<block>> blocks;
	std::shared_ptr<block> currentBlock;
	unsigned int currentStudent = 0;

	commandType currentCommand;

	tree<std::string, std::pair<std::shared_ptr<block>, unsigned int>> tree;
};

commandType studentRecord::generateCommand(std::string input) {
	if (input == "INSERT") {
		return INSERT;
	} else if (input == "UPDATE") {
		return UPDATE;
	} else if (input == "SEARCH") {
		return SEARCH;
	} else if (input == "DELETE") {
		return DELETE;
	} else if (input == "LIST") {
		return LIST;
	} else if (input == "SNAPSHOT") {
		return SNAPSHOT;
	} else {
		return NONE;
	}
}

bool studentRecord::handleCommand(const commandType command, const std::string key, const std::string value) {
	switch (command) {
	case INSERT:
		return this->insert(key);
		break;
	case UPDATE:
		return this->update(key, value);
		break;
	case SEARCH:
		return this->search(key);
		break;
	case DELETE:
		return this->remove(key);
		break;
	case LIST:
		this->list();
		return true;
		break;
	case SNAPSHOT:
		this->snapshot();
		return true;
		break;
	case NONE:
	default:
		return false;
		break;
	}
}

bool studentRecord::insert(const std::string key) {
	// See if we need to make the first block
	if (this->currentBlock == nullptr) {
		this->currentBlock = std::make_shared<block>();
		this->currentStudent = 0;
	}

	// See if it already exists within the tree
	if (!tree.exists(key)) {
		// Create a pair to use for inserting
		auto pair = std::pair<std::shared_ptr<block>, unsigned int>(this->currentBlock, this->currentStudent);
		pair.first->setStudentName(this->currentStudent, key);

		// Check to see if we have to make a new block
		if (this->currentStudent == BLOCK_SIZE - 1) {
			this->blocks.push_back(this->currentBlock);
			this->currentBlock = std::make_shared<block>();
			this->currentStudent = 0;
		}
		else {
			this->currentStudent++;
		}

		return this->tree.insert(key, pair);
	} else {
#if FILEOUTPUT
		output << "Unable to insert: " << key << ": already exists in tree" << std::endl;
#endif
		std::cerr << "Unable to insert: " << key << ": already exists in tree" << std::endl;
		return false;
	}
}

bool studentRecord::update(const std::string key, const std::string value) {
	if (this->tree.exists(key)) {
		auto block = this->tree.search(key);

		// Check to see if we found it in the tree
		if (block != nullptr) {
			// Clear it out and update the data
			block->first->clearStudentInformation(block->second);
			block->first->setStudentInformation(block->second, value);

			// Update our tree with this information
			return this->tree.update(key, *block);
		}
	} else {
#if FILEOUTPUT
		output << "Unable to update: " << key << ": does not exist in tree" << std::endl;
#endif
		std::cerr << "Unable to update: " << key << ": does not exist in tree" << std::endl;
		return false;
	}
}

bool studentRecord::search(const std::string key) {
	if (this->tree.exists(key)) {
		auto block = this->tree.search(key);

		// Check to see if we found it in the tree
		if (block != nullptr) {
			// Output the result
			std::cout << block->first->getStudentInformation(block->second) << std::endl;
			return true;
		}
	} else {
#if FILEOUTPUT
		output << "Unable to search: " << key << ": does not exist in tree" << std::endl;
#endif
		std::cerr << "Unable to search: " << key << ": does not exist in tree" << std::endl;
		return false;
	}
}

bool studentRecord::remove(const std::string key) {
	if (this->tree.exists(key)) {
		auto block = this->tree.search(key);

		// Check to see if it exists within the tree
		if (block != nullptr) {
			// Clear out the information
			block->first->clearStudentName(block->second);
			block->first->clearStudentInformation(block->second);

			// Remove the key from the tree
			this->tree.remove(key);

			// Check to see if we need to remove a block
			if (block->first->blockEmpty()) {
				auto location = std::find(blocks.begin(), blocks.end(), block->first);
				if (location != blocks.end()) {
					blocks.erase(location);
				}
			}

			return true;
		}
	} else {
#if FILEOUTPUT
		output << "Unable to delete: " << key << ": does not exist in tree" << std::endl;
#endif
		std::cerr << "Unable to delete: " << key << ": does not exist in tree" << std::endl;
		return false;
	}
}

void studentRecord::list() {
#if FILEOUTPUT
	output << "***** LIST *****" << std::endl;
#endif
	std::cout << "***** LIST *****" << std::endl;
	this->tree.list();
#if FILEOUTPUT
	output << "***** LIST *****" << std::endl;
#endif
	std::cout << "***** LIST *****" << std::endl;
}

void studentRecord::snapshot() {
	unsigned int numberOfBlocks = blocks.size();
	if (this->currentBlock != nullptr) {
		numberOfBlocks++;
	}
#if FILEOUTPUT
	output << std::endl << "***** SNAPSHOT *****" << std::endl;
	output << "Number of elements: " << this->tree.getSize() << std::endl;
	output << "Number of blocks used: " << numberOfBlocks << std::endl;
	output << "Depth of tree: " << this->tree.getDepth() << std::endl;
	output << "Node Keys: " << std::endl << std::endl;
#endif
	std::cout << std::endl << "***** SNAPSHOT *****" << std::endl;
	std::cout << "Number of elements: " << this->tree.getSize() << std::endl;
	std::cout << "Number of blocks used: " << numberOfBlocks << std::endl;
	std::cout << "Depth of tree: " << this->tree.getDepth() << std::endl;
	std::cout << "Node Keys: " << std::endl << std::endl;

	this->tree.snapshot();

#if FILEOUTPUT
	output << std::endl << "***** SNAPSHOT *****" << std::endl;
#endif
	std::cout << std::endl << "***** SNAPSHOT *****" << std::endl;
}

#endif // PROJECT_2_STUDENT_RECORD_HPP