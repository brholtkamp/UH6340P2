//
// Created by Brian on 4/15/2015.
//

#ifndef PROJECT_2_TREE_HPP
#define PROJECT_2_TREE_HPP

#include <memory>

#include "Node/node.hpp"
#include "Node/internalNode.hpp"
#include "Node/leafNode.hpp"

template<typename K, typename V>
class tree {
public:
    tree() : rootNode(std::make_shared<leafNode<K, V>>()) { }

    bool insert(const K key, const V value);
    std::shared_ptr<node<K, V>> search(const K key);
    bool exists(const K key);
    void list();
    bool update(const K key, const V value);
    bool remove(const K key);

    ~tree() { };

private:
    int degree;
    std::shared_ptr<node<K, V>> rootNode;
};

template <typename K, typename V>
bool tree<K, V>::insert(const K key, const V value) {
    // Check to make sure this is a unique key
    if (exists(key)) {
        std::cerr << "Key: " << key << " already exists within the tree, insertion failed" << std::endl;
        return false;
    }

    // Insert into the tree and record the result
    auto results = this->rootNode->insert(key, value);

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
std::shared_ptr<node<K, V>> tree<K, V>::search(const K key) {
    return this->rootNode->search(key);
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
    std::cout << "Tree Structure: " << std::endl;
    this->rootNode->list(0);

    std::cout << std::endl << "Leaf Structure: " << std::endl;
    auto node = this->rootNode;
    while (node->getType() != LEAF) {
        node = std::dynamic_pointer_cast<internalNode<K, V>>(node)->children[0];
    }
    
    unsigned int total = 0;
    while (node != nullptr) {
        for (unsigned int i = 0; i < node->numberOfKeys; i++) {
            std::cout << node->keys[i] << " ";
            total++;
        }
        node = std::dynamic_pointer_cast<leafNode<K, V>>(node)->nextLeaf;
    }
    std::cout << std::endl << "Total: " << total << std::endl;
}

template <typename K, typename V>
bool tree<K, V>::update(const K key, const V value) {
    return this->rootNode->update(key, value);
}

template <typename K, typename V>
bool tree<K, V>::remove(const K key) {
    return this->rootNode->remove(key);
}

#endif //PROJECT_2_TREE_HPP
