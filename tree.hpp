#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <memory>

#define debug 1

enum nodeType {
    INTERNAL,
    LEAF
};

// Node template for the other 2 nodes
template <typename T>
class node {
public:
    node(int degree, nodeType type) : degree(degree), type(type) { };

    bool withinRange(T value);
    nodeType getType();



    // TODO: Finish the rule of five for node
    node(const node& otherNode) = default;
    node(node&& otherNode) = default;
    node& operator=(const node& otherNode) = default;
    node& operator=(node&& otherNode) = default;
    ~node() { };

private:
    int degree;
    nodeType type;
    std::map<T, std::unique_ptr<node<T>>> map;
};

template <typename T>
class tree {
public:
    tree<T>(int degree) : degree(degree) { };

    bool insert(T input);
    bool remove(T input);
    bool exists(T input);
    void list();
    bool update(T input);
    std::unique_ptr<node<T>> search(T input);

    bool isEmpty() { return root == nullptr; }

    // TODO: Finish the rule of five
    tree(const tree& otherTree) = default;
    tree(tree&& otherTree) = default;
    tree& operator=(const tree& otherTree) = default;
    tree& operator=(tree&& otherTree) = default;
    ~tree() { };

private:
    std::unique_ptr<node<T>> treeSearch(T input, std::unique_ptr<node<T>> currentNode);

    int degree;
    std::unique_ptr<node<T>> root;
};


template <typename T>
bool node<T>::withinRange(T value) {
    // Check to see if there is an element below value
    auto lower = map.lower_bound(value);
    // Check to see if there is an element above value
    auto upper = map.upper_bound(value);

    // Check to see if any results occurred
    if (lower != map.end() && upper != map.end()){
        return true;
    }

    return false;
}

template <typename T>
nodeType node<T>::getType() {
    return type;
}

template<typename T>
bool tree<T>::insert(T input) {
    // Check to make sure the root is initialized
    if (isEmpty()) {
        root = std::unique_ptr<node<T>>(new node<T>(degree, INTERNAL));
    }

    std::unique_ptr<node<T>> location = search(input);
    if (location != nullptr) {
        // There exists an item within the tree
        return true;
    }

    return false;
}

template <typename T>
bool tree<T>::remove(T input) {
    return false;
}

template <typename T>
bool tree<T>::exists(T input) {
    return false;
}

template <typename T>
void tree<T>::list() {

}

template <typename T>
bool tree<T>::update(T input) {
    return false;
}

template <typename T>
std::unique_ptr<node<T>> tree<T>::search(T input) {
    return nullptr;
}

template <typename T>
std::unique_ptr<node<T>> tree<T>::treeSearch(T input, std::unique_ptr<node<T>> currentNode) {
    if (currentNode->getType() == LEAF) {
       return currentNode;
    } else if (currentNode->withinRange(input)) {

    } else {

    }
}
