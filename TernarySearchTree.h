//
// Created by Preston on 4/16/2021.
//
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
using namespace std;
#ifndef CS415PROJECT03_TERNARYSEARCHTREE_H
#define CS415PROJECT03_TERNARYSEARCHTREE_H

//// Reason for this being struct Noode rather than Node is that in Trie.h I have another struct Node
//// and since structs are public by dafault the compiler was not happy with 2 diff struct Nodes.
struct Noode{
        char c;
        bool endOfString;
        struct Noode *left;
        struct Noode *right;
        struct Noode *down;
};

class TernarySearchTree {
public:
    TernarySearchTree();
    struct Noode * create(char & t);
    struct Noode * insert(struct Noode * currRoot, string & s, int & idx);
    Noode * root = nullptr;
    void traverseHelper(struct Noode * nod, char * buffer, int lvl);
    void inOrder();
    bool search(struct Noode * nod, string & s, int idx, char * buffer, int lvl);
    bool searchNoAutoComplete(struct Noode * nod, string & s, int idx);
    vector<string> autoCompletes;
    double autoCompleteTime = 0;

};


#endif //CS415PROJECT03_TERNARYSEARCHTREE_H
