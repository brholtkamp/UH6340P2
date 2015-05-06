#ifndef PROJECT_2_LEAFNODE_HPP
#define PROJECT_2_LEAFNODE_HPP

#include <vector>
#include <memory>
#include <algorithm>

#include "node.hpp"

template <typename K, typename V>
class leafNode : public node < K, V > {
public:
    leafNode() : node<K, V>(), nextLeaf(nullptr) { }

    std::shared_ptr<node<K, V>> findNode(const K key) override;
    nodeType getType() override { return LEAF; }

    std::unique_ptr<split<K, V>> insert(const K key, const V value) override;
    std::shared_ptr<node<K, V>> search(const K key) override;
    void list(unsigned int depth) override;
    bool update(const K key, const V value) override;
    bool remove(const K key) override;

    std::array<V, DEFAULT_DEGREE + 1> values;
    std::shared_ptr<leafNode<K, V>> nextLeaf;
};

template <typename K, typename V>
std::shared_ptr<node<K, V>> leafNode<K, V>::findNode(const K key) {
    // If this is reached, this is the most applicable child
    return this->shared_from_this();
}

template <typename K, typename V>
std::unique_ptr<split<K, V>> leafNode<K, V>::insert(const K key, const V value) {
    // Find the proper index for this key
    unsigned int index = this->findIndex(key);

    // Check to see if it's currently full
    if (this->numberOfKeys == DEFAULT_DEGREE) {
#if DEBUG
        std::cout << "Leaf overflowed, having to split: "; this->list(0);
#endif
        // Figure out the middle of this nodee
        unsigned int middleIndex = (DEFAULT_DEGREE + 1) / 2;

        // Create a new leaf
        auto newLeaf = std::make_shared<leafNode<K, V>>();

        // Copy in the values from the right half of this node
        for (unsigned int i = 0; i < middleIndex; i++) {
            newLeaf->numberOfKeys++;
            newLeaf->keys[i] = this->keys[middleIndex + i];
            newLeaf->values[i] = this->values[middleIndex + i];
        }
        // Update the number of keys to "erase" the previous entries
        this->numberOfKeys = middleIndex;

        // Determine if the new key should go to the left or right
        if (index < middleIndex) {
            this->insert(key, value);
        }
        else {
            newLeaf->insert(key, value);
        }

#if DEBUG
        std::cout << "Left: "; this->list(0); std::cout << "Right: "; newLeaf->list(0);
#endif
        // Create the split struct to send back the new configuration
        auto splitResult = std::unique_ptr<split<K, V>>(new split<K, V>());
        splitResult->key = newLeaf->keys[0];
        splitResult->left = this->shared_from_this();
        splitResult->right = newLeaf;
        
        // String together the leaves
        this->nextLeaf = newLeaf;

        return splitResult;
    }
    else {
        // Check for the first item instance
        if (this->numberOfKeys == 0) {
            this->numberOfKeys++;
            this->keys[0] = key;
            this->values[0] = value;
        }
        else {
            // Just add into the proper location and move items over
            for (unsigned int i = this->numberOfKeys; i > index; i--) {
                this->keys[i] = this->keys[i - 1];
                this->values[i] = this->values[i - 1];
            }
            this->numberOfKeys++;
            this->keys[index] = key;
            this->values[index] = value;
        }

        // No new nodes were made, so just return nullptr
        return nullptr;
    }
}

template <typename K, typename V>
std::shared_ptr<node<K, V>> leafNode<K, V>::search(const K key) {
    return this->shared_from_this();
}

template <typename K, typename V>
void leafNode<K, V>::list(unsigned int depth) {
    std::string buffer;
    for (unsigned int i = 0; i < depth; i++) {
        buffer += "\t";
    }

    std::cout << buffer;
    for (unsigned int i = 0; i < this->numberOfKeys; i++) {
        std::cout << this->keys[i] << ":" << this->values[i] << " ";
    }
    std::cout << std::endl;
}

template <typename K, typename V>
bool leafNode<K, V>::update(const K key, const V value) {
    unsigned int index = this->findIndex(key);
    if (this->keys[index] == key) {
        this->values[index] = value;
        return true;
    }

    return false;
}

template <typename K, typename V>
bool leafNode<K, V>::remove(const K key) {
    unsigned int index = this->findIndex(key);
    if (this->keys[index] == key) {
        for (unsigned int i = index; i < this->numberOfKeys; i++) {
            this->keys[i] = this->keys[i + 1];
            this->values[i] = this->values[i + 1];
        }

        this->numberOfKeys--;
        return true;
    }

    return false;
}

#endif // PROJECT_2_LEAFNODE_HPP
