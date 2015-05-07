//
// Created by Brian on 4/15/2015.
//

#ifndef PROJECT_2_TREE_HPP
#define PROJECT_2_TREE_HPP

#define FILE_OUTPUT 1

const unsigned int OUTPUT_WIDTH = 16;
const unsigned int DEFAULT_DEGREE = 10;

#include <memory>
#include <fstream>
#include <iomanip>

#include "node.hpp"
#include "internalNode.hpp"
#include "leafNode.hpp"

std::ofstream outputFile;

template<typename K, typename V>
class tree {
public:
    tree() : rootNode(nullptr) {
#if FILE_OUTPUT
        outputFile.open("out.txt");
#endif
    }

    bool insert(const K key, const V value);
    std::shared_ptr<V> search(const K key);
    bool exists(const K key);
    void list();
    void snapshot();
    bool update(const K key, const V value);
    bool remove(const K key);

    inline unsigned int getSize() { return size; }
    inline unsigned int getDepth() { return this->rootNode != nullptr ? this->rootNode->getDepth(0) : 0; }

    ~tree() {
#if FILE_OUTPUT
        outputFile.close();
#endif
    }

private:
    int degree;
    unsigned int size = 0;
    std::shared_ptr<node<K, V>> rootNode;
};

template<typename K, typename V>
bool tree<K, V>::insert(const K key, const V value) {
    // See if we have an empty tree
    if (this->rootNode == nullptr) {
        this->rootNode = std::make_shared<leafNode<K, V>>();
    }

    // Check to make sure this is a unique key
    if (this->exists(key)) {
        return false;
    }

    // Insert into the tree and record the result
    auto results = this->rootNode->insert(key, value);

    // Increment our number of inserted items
    this->size++;

    // If we don't receive nullptr, we split and it propagated up to the root
    if (results != nullptr) {
        auto newRoot = std::make_shared<internalNode<K, V>>();
        newRoot->numberOfKeys = 1;
        newRoot->keys[0] = results->key;
        newRoot->children[0] = results->left;
        newRoot->children[1] = results->right;
        this->rootNode = newRoot;
    }

    return true;
}

template<typename K, typename V>
std::shared_ptr<V> tree<K, V>::search(const K key) {
    if (this->exists(key) && this->rootNode != nullptr) {
        auto node = this->rootNode->search(key);
        if (node != nullptr) {
            std::shared_ptr<V> result = std::make_shared<V>(
                    std::dynamic_pointer_cast<leafNode<K, V>>(node)->values[node->findIndex(key)]);
            if (result != nullptr) {
                return result;
            }
        }
    }

    return nullptr;
}

template<typename K, typename V>
bool tree<K, V>::exists(const K key) {
    if (this->rootNode != nullptr) {
        auto results = this->rootNode->search(key);
        if (results != nullptr) {
            return std::find(results->keys.begin(), results->keys.end(), key) != results->keys.end();
        }
    }

    return false;
}

template<typename K, typename V>
void tree<K, V>::list() {
    if (this->rootNode != nullptr) {
        auto currentNode = this->rootNode;
        while (currentNode->getType() != LEAF) {
            auto currentInternal = std::dynamic_pointer_cast<internalNode<K, V>>(currentNode);
            currentNode = std::dynamic_pointer_cast<node<K, V>>(currentInternal->children[0]);
        }

        auto currentLeaf = std::dynamic_pointer_cast<leafNode<K, V>>(currentNode);
        while (currentLeaf != nullptr) {
            for (unsigned int i = 0; i < currentLeaf->numberOfKeys; i++) {
#if FILE_OUTPUT
                outputFile << currentLeaf->keys[i] << ", ";
#endif
                std::cout << currentLeaf->keys[i] << ", ";
            }
            currentLeaf = currentLeaf->nextLeaf;
        }
    }
}

template<typename K, typename V>
void tree<K, V>::snapshot() {
    if (this->rootNode != nullptr) {
        this->rootNode->snapshot(0);
    }
}

template<typename K, typename V>
bool tree<K, V>::update(const K key, const V value) {
    if (this->rootNode != nullptr) {
        if (this->exists(key)) {
            return this->rootNode->update(key, value);
        }
    }

    return false;
}

template<typename K, typename V>
bool tree<K, V>::remove(const K key) {
    if (this->rootNode != nullptr) {
        if (!this->exists(key)) {
            return false;
        }

        // Decrement our size
        size--;

        // Return the result of the removal
        bool result = this->rootNode->remove(key);

        // Check to see if the root is empty
        if (this->rootNode->numberOfKeys == 0) {
            this->rootNode = nullptr;
        }

        return result;
    }

    return false;
}

#endif //PROJECT_2_TREE_HPP
