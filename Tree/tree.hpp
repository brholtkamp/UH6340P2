//
// Created by Brian on 4/15/2015.
//

#ifndef PROJECT_2_TREE_HPP
#define PROJECT_2_TREE_HPP

const unsigned int OUTPUT_WIDTH = 16;

#include <memory>
#include <fstream>
#include <iomanip>

#include "Node/node.hpp"
#include "Node/internalNode.hpp"
#include "Node/leafNode.hpp"

template<typename K, typename V>
class tree {
public:
    tree() : rootNode(std::make_shared<leafNode<K, V>>()) { }

    bool insert(const K key, const V value);
    std::shared_ptr<V> search(const K key);
    bool exists(const K key);
    void list();
    bool update(const K key, const V value);
    bool remove(const K key);

	inline unsigned int getSize() { return size; }
	inline unsigned int getDepth() { return this->rootNode->getDepth(0); }

    ~tree() { }

private:
    int degree;
	unsigned int size = 0;
    std::shared_ptr<node<K, V>> rootNode;
};

template <typename K, typename V>
bool tree<K, V>::insert(const K key, const V value) {
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

template <typename K, typename V>
std::shared_ptr<V> tree<K, V>::search(const K key) {
	if (this->exists(key)) {
		auto node = this->rootNode->search(key);
		if (node != nullptr) {
			std::shared_ptr<V> result = std::make_shared<V>(std::dynamic_pointer_cast<leafNode<K, V>>(node)->values[node->findIndex(key)]);
			if (result != nullptr) {
				return result;
			}
		}
	}

	return nullptr;
}

template <typename K, typename V>
bool tree<K, V>::exists(const K key) {
    auto results = this->rootNode->search(key);
    if (results != nullptr) {
        return std::find(results->keys.begin(), results->keys.end(), key) != results->keys.end();
    }
    else {
        return false;
    }
}

template <typename K, typename V>
void tree<K, V>::list() {
    this->rootNode->list(0);
}

template <typename K, typename V>
bool tree<K, V>::update(const K key, const V value) {
	if (!this->exists(key)) {
		return false;
	}

    return this->rootNode->update(key, value);
}

template <typename K, typename V>
bool tree<K, V>::remove(const K key) {
	if (!this->exists(key)) {
		return false;
	}

	// Decrement our size
	size--;

	// Return the result of the removal
    return this->rootNode->remove(key);
}

#endif //PROJECT_2_TREE_HPP
