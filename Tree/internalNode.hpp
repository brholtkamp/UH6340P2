//
// Created by Brian on 4/15/2015.
//

#ifndef PROJECT_2_INTERNALNODE_HPP
#define PROJECT_2_INTERNALNODE_HPP

#include <vector>
#include <memory>
#include <algorithm>

#include <iostream>
#include <fstream>
#include <iomanip>

#include "tree.hpp"
#include "node.hpp"
#include "leafNode.hpp"

extern std::ofstream outputFile;

template<typename K, typename V>
class internalNode : public node<K, V> {
public:
    internalNode() : node<K, V>() { }

    std::unique_ptr<split<K, V>> insert(const K key, const V value) override;
    std::shared_ptr<node<K, V>> search(const K key) override;
    void snapshot(unsigned int depth) override;
    bool update(const K key, const V value) override;
    bool remove(const K key) override;

    std::shared_ptr<node<K, V>> findNode(const K key) override;
    unsigned int findIndex(const K key) override;
    unsigned int getDepth(unsigned int depth) override { return this->children[0]->getDepth(depth + 1); }
    nodeType getType() override { return INTERNAL; }

    std::array<std::shared_ptr<node<K, V>>, DEFAULT_DEGREE + 1> children;
};

template<typename K, typename V>
std::shared_ptr<node<K, V>> internalNode<K, V>::findNode(const K key) {
    // Tell that child to find the node
    return this->children[this->findIndex(key)]->findNode(key);
}

template<typename K, typename V>
unsigned int internalNode<K, V>::findIndex(const K key) {
    unsigned int i = 0;

    while (i < this->numberOfKeys && this->keys[i] <= key) {
        i++;
    }

    return i;
}

template<typename K, typename V>
std::unique_ptr<split<K, V>> internalNode<K, V>::insert(const K key, const V value) {
    // Check to see if this node needs to be split
    if (this->numberOfKeys == DEFAULT_DEGREE) {
        // Figure out the middle of this node
        unsigned int middleIndex = (DEFAULT_DEGREE + 1) / 2;

        // Create a new internal node for the right
        auto newInternalNode = std::make_shared<internalNode<K, V>>();
		newInternalNode->numberOfKeys = middleIndex;

        // Populate the new internal node with the right keys and children
        for (unsigned int i = 0; i < middleIndex; i++) {
            newInternalNode->keys[i] = this->keys[middleIndex + i];
            newInternalNode->children[i] = this->children[middleIndex + i];
        }

        // Catch the final child on the furthest right
        newInternalNode->children[newInternalNode->numberOfKeys] = this->children[this->numberOfKeys];

        // Release the pointers to the now non-children
        for (unsigned int i = middleIndex; i < this->numberOfKeys; i++) {
            this->children[i] = nullptr;
        }

        // Update the number of keys to reflect the split
        this->numberOfKeys = middleIndex - 1;

        // Setup the split results to send back
        auto splitResult = std::unique_ptr<split<K, V>>(new split<K, V>());
        splitResult->key = this->keys[middleIndex - 1];
        splitResult->left = this->shared_from_this();
        splitResult->right = newInternalNode;

        // Add in the new key
        if (key < splitResult->key) {
            this->insert(key, value);
        }
        else {
            newInternalNode->insert(key, value);
        }

        return splitResult;
    }
    else {
        // Find the best index for this node
        unsigned int index = this->findIndex(key);

        // Insert into that child and record the result
        auto results = this->children[index]->insert(key, value);

        // If we don't receive nullptr, the child split and we need to update
        if (results != nullptr) {
			unsigned int index = this->findIndex(key);

            // Move the last child over 1 to make room
            this->children[this->numberOfKeys + 1] = this->children[this->numberOfKeys];

            // Iterate through and move the children and keys over
            for (unsigned int i = this->numberOfKeys; i != index; --i) {
                this->keys[i] = this->keys[i - 1];
                this->children[i] = this->children[i - 1];
            }

            this->keys[index] = results->key;
            this->children[index] = results->left;
            this->children[index + 1] = results->right;
            this->numberOfKeys++;
        }

        return nullptr;
    }
}

template<typename K, typename V>
std::shared_ptr<node<K, V>> internalNode<K, V>::search(const K key) {
    return this->children[this->findIndex(key)]->search(key);
}

template<typename K, typename V>
void internalNode<K, V>::snapshot(unsigned int depth) {
    std::string buffer;
    for (unsigned int i = 0; i < depth; i++) {
        buffer += "\t";
    }
#if FILE_OUTPUT
    if (this->numberOfKeys > 1) {
        outputFile << buffer << std::left << std::setw(OUTPUT_WIDTH) << this->keys[0] << " <-> " <<
        std::setw(OUTPUT_WIDTH) << this->keys[this->numberOfKeys - 1] << std::endl;
    } else {
        outputFile << buffer << std::left << std::setw(OUTPUT_WIDTH) << this->keys[0] << " <-> " <<
        std::setw(OUTPUT_WIDTH) << this->keys[0] << std::endl;
    }
#endif
    if (this->numberOfKeys > 1) {
        std::cout << buffer << std::left << std::setw(OUTPUT_WIDTH) << this->keys[0] << " <-> " <<
        std::setw(OUTPUT_WIDTH) << this->keys[this->numberOfKeys - 1] << std::endl;
    } else {
        std::cout << buffer << std::left << std::setw(OUTPUT_WIDTH) << this->keys[0] << " <-> " <<
        std::setw(OUTPUT_WIDTH) << this->keys[0] << std::endl;
    }

    for (unsigned int i = 0; i <= this->numberOfKeys; i++) {
        this->children[i]->snapshot(depth + 1);
    }
}

template<typename K, typename V>
bool internalNode<K, V>::update(const K key, const V value) {
    return this->children[this->findIndex(key)]->update(key, value);
}

template<typename K, typename V>
bool internalNode<K, V>::remove(const K key) {
    // Tell that child to remove that key
    unsigned int index = this->findIndex(key);
    bool result = this->children[index]->remove(key);

    // Make sure the removal happened and check to see if the node should be removed (has no more keys)
    if (this->children[index]->numberOfKeys == 0 && result) {
        // Update the linked leaves structure
        if (this->children[index]->getType() == LEAF) {
			auto currentLeaf = std::dynamic_pointer_cast<leafNode<K, V>>(this->children[index]);
			auto previousLeaf = currentLeaf->previousLeaf;
			auto nextLeaf = currentLeaf->nextLeaf;
			previousLeaf->nextLeaf = nextLeaf;
			nextLeaf->previousLeaf = previousLeaf;
        }

        // Iterate through and replace the previous values
        for (unsigned int i = index; i < this->numberOfKeys; i++) {
            this->keys[i] = this->keys[i + 1];
            this->children[i] = this->children[i + 1];
        }

        // Track the deletion
        this->numberOfKeys--;
    }

    return result;
}

#endif // PROJECT_2_INTERNALNODE_HPP
