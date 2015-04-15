#include <iostream>
#include <string>
#include "tree.hpp"

using namespace std;

int main() {
    node<int> node1(3, LEAF);

    tree<std::string> tree(11);
    std::string input = "Hi";
    if (tree.insert(input)) {
        std::cout << "Success!" << std::endl;
    }

    cout << "Hello, World!" << endl;
    return 0;
}