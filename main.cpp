//
// Created by Brian on 4/15/2015.
//

#include <iostream>
#include <string>

#include "Tree/tree.hpp"

int main() {
    tree<std::string, std::string> tree;
    tree.insert("ABC", "ABC"); // 1
    tree.insert("BCD", "BCD"); // 2
    tree.insert("CDE", "CDE"); // 3
    tree.insert("AAA", "AAA"); // 4
    tree.insert("ABB", "ABB"); // 5
    tree.insert("ABC", "ABC"); // Dupe
    tree.insert("BBB", "BBB"); // 6
    tree.insert("CCC", "CCC"); // 7
    tree.insert("DDD", "DDD"); // 8
    tree.insert("EEE", "EEE"); // 9
    tree.insert("FFF", "FFF"); // 10
    tree.insert("GGG", "GGG"); // 11
    tree.insert("HHH", "HHH"); // 12
    tree.insert("BBC", "BBC"); // 13
    tree.insert("BBD", "BBD"); // 14
    tree.insert("BBE", "BBE"); // 15
    tree.insert("BBF", "BBF"); // 16
    tree.insert("BBG", "BBG"); // 17
    tree.insert("BBH", "BBH"); // 18
    tree.insert("BBI", "BBI"); // 19
    tree.insert("BBJ", "BBJ"); // 20
    tree.insert("AAB", "AAB"); // 21
    tree.insert("AAC", "AAC"); // 22
    tree.insert("AAD", "AAD"); // 23
    tree.insert("AAE", "AAE"); // 24
    tree.insert("AAF", "AAF"); // 25
    tree.insert("AAG", "AAG"); // 26
    tree.insert("AAH", "AAH"); // 27
    tree.list();
    tree.update("ABC", "!!!");
    tree.update("AAH", "!!!");
    tree.update("BBJ", "!!!!!");
    tree.list();
    tree.remove("HHH");
    tree.remove("AAA");
    tree.remove("AAB");
    tree.remove("AAC");
    tree.remove("AAD");
    tree.remove("AAE");
    tree.remove("AAH");
    tree.list();

    std::string input = "";
    while (input != "exit") {
        std::cin >> input;
    }

    return 0;
}
