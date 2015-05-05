#ifndef PROJECT_2_NODE_HPP
#define PROJECT_2_NODE_HPP

#define DEBUG 0

#include <array>
#include <memory>

const unsigned int DEFAULT_DEGREE = 10;

enum nodeType {
    INTERNAL,
    LEAF
};

template <typename K, typename V>
class node;

template <typename K, typename V>
class split {
public:
    K key;
    std::shared_ptr<node<K, V>> left;
    std::shared_ptr<node<K, V>> right;
};

template <typename K, typename V>
class node : public std::enable_shared_from_this<node<K, V>> {
public:
    node(): numberOfKeys(0) { }

    virtual std::shared_ptr<node<K, V>> findNode(const K key) = 0;
    virtual nodeType getType() = 0;

    virtual std::unique_ptr<split<K, V>> insert(const K key, const V value) = 0;
    virtual std::shared_ptr<node<K, V>> search(const K key) = 0;
    virtual void list(unsigned int depth) = 0;
    virtual bool update(const K key, const V value) = 0;
    virtual bool remove(const K key) = 0;

    virtual ~node() { }

    unsigned int findIndex(const K key);

    unsigned int numberOfKeys;
    std::array<K, DEFAULT_DEGREE> keys;
};

template <typename K, typename V>
unsigned int node<K, V>::findIndex(const K key) {
    // Check for the empty node
    if (numberOfKeys == 0) {
        return 0;
    }

    // Attempt to find the next biggest key
    for (unsigned int i = 0; i < numberOfKeys; i++) {
        if (keys[i] > key) {
            return i;
        }
    }

    // Return the largest index since it's bigger than all previous keys
    return static_cast<unsigned int>(numberOfKeys);
}

#endif // PROJECT_2_NODE_HPP