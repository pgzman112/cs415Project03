//
// Created by Preston on 4/16/2021.
//
#include <string>
#include <chrono>
using namespace std;

#ifndef CS415PROJECT03_TRIE_H
#define CS415PROJECT03_TRIE_H
#include <vector>

struct Node{
    struct Node *children[26];
    int numTerminatingStrings;
};

class Trie {
    public:
        Trie();
        void insert(string s);
        static struct Node *createNode();
        void print(struct Node *currRoot, char str[], int level);
        bool search(string temp, bool pred);
        Node * root;
        vector<string> printed;
         double searchTime;
         double autoCompleteTime = 0;
    private:
       // Node * root;

};


#endif //CS415PROJECT03_TRIE_H
