//
// Created by Preston on 4/16/2021.
//
#include <iostream>
#include "Trie.h"
#include <string>
#include <chrono>
#include <vector>
#include <algorithm>

using namespace std;

// Default constructor
Trie::Trie(){
    // c++ 20 and clion seem to really like auto rather than
    // struct Node *root = new Node;
    // creating our root node.
    auto *node = new Node;
    //Setting num terminating strings to 0 although nothing really terminates at the root anyways
    node->numTerminatingStrings = 0;
    // Initializing all the child pointers to NULL
    for(auto & i : node->children){
        i = nullptr;
    }
    root = node;
}

// Insert function
void Trie::insert(string s){
    // Create a new node called iter to iterate through our tree.
    struct Node *iter = root;
    //Loop through each character in our string that we want to input
    for(int i = 0; i < s.length(); i++){
        // the index we want to check is 0-26 so to get that from our char
        // we just subtract it from the char 'a' with an ascii value of 97
        // so if we have a we take a-a = 0 and check index 0, b-a=1 so check idx 1.
        // and so on
        int idx = s[i] - 'a';
        //cout << "idx: " << idx << endl;
        // IF this index has not been touched ie: it is null, we create a new node
        // at the child[idx]
        if(iter->children[idx] == nullptr){
            //cout << "creating new node" << endl;
            iter->children[idx] = createNode();
        }
        // set iter to the idx of children to continue traversing the string.
        iter = iter->children[idx];
    }
    // Once we have made it to the end of the string we complete the insert by
    // Increasing the number of strings that terminate at the end of this string.
    // This could just be a bool value to signify that this string exists in our trie
    // But I like the idea of keeping track of how many times a string is attempted
    // to be added as extra functionality.
    iter->numTerminatingStrings++;
}

// Creates a new empty node initialized to nulls.
struct Node * Trie::createNode() {
    // Create new empty node
    auto *newNode = new Node;
    // Set num string Terminated to zero
    newNode->numTerminatingStrings = 0;
    // Initialize child pointers to NULL
    for(auto & i : newNode->children){
        i = nullptr;
    }
    // return the newNode
    return newNode;
}

// Function that can print out entire Trie tree OR all strings stemming from some node
void Trie::print(struct Node *currRoot, char str[], int lvl){
    // string s contains any predicates this will be used later.
    string temp;
    //Create an iterator of current node
    auto *iter = currRoot;
    // Is this node the end of a word? if so lets push it back onto the vector of total strings!
    if(iter->numTerminatingStrings > 0){
        str[lvl] = '\0';
        string temp(str);
        //cout << "placing: " << temp << endl;
        if(count(printed.begin(), printed.end(), temp) || temp.empty()){
            // do nothing because already contains it.
            // This is a special case because I am a terrible programmer and implemented
            // search in a strange way. this keeps it from having duplicates though.
        }
        else {
            printed.emplace_back(temp);
        }
    }
    //int i;
    for(int i = 0; i < 26; i++){
        // Check for all strings that stem out from current node!
        if(currRoot->children[i] != nullptr){
            // PushBack the current char at the current placeholder
            str[lvl] = char(i + 97);
            // Recursively call print to print the rest of the tree!
            print(currRoot->children[i], str, lvl+1);
        }
    }
}

// Function that can search for a particular string OR can also be used to
// Find a predicate and then print all strings after it.
// need to call printed.clear() after this function is called back in main.
bool Trie::search(string temp, bool pred){
    // this is for if we are searching for all strings that start with
    // A particular predicate!
    if(pred){
        // set an iterator to the root
        auto *iter = root;
        // Loop through the predicate string and trace through our Trie data structure
        // looking to see if we can match the predicate. returns false if it cant
        // match anything to our predicate.
        for(auto i = 0; i < temp.length(); i++){
            // we want the index of the char between 0 -> 26
            int idx = temp[i]-97;
            // If the index is not a null pointer in the child array they we've found the curr char
            if(iter->children[idx] != nullptr){
                iter = iter->children[idx];
            }
            else{
                // didnt find the curr char in the child array
                return false;
            }
        }
        if(iter->numTerminatingStrings > 0){
            printed.emplace_back(temp);
        }
        auto t1 = chrono::high_resolution_clock::now();
        int lvl = 0;
        char str[100];
        print(iter, str, lvl);
        auto t2 = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> totalTime = t2-t1;
        this->autoCompleteTime = totalTime.count();
        // If we got to here but the vector printed is empty
        // That means that there were no strings which stemmed from the predicate
        // Therefore return false else return true;
        if(printed.empty()){
            return false;
        }
        else{
            return true;
        }
    }
    // If we are searching for one particular string.
    else{
        auto *iter = root;
        // This will loop through the temp string tracing it. if it ever sees
        // that we dont have the correct char it returns false
        for(auto i = 0; i < temp.length(); i++){
            // we want the index of the char between 0 -> 26
            int idx = temp[i]-97;
            // If the index is not a null pointer in the child array they we've found the curr char
            if(iter->children[idx] != nullptr){
                iter = iter->children[idx];
            }
            else{
                // didnt find the curr char in the child array
                return false;
            }
        }
        // if the node we're at has a terminated string at its position then we have successfully
        // traced the searched word AND landed on a word that is in our trie data structure therefore
        // return true else return false
        if(iter->numTerminatingStrings > 0){
            return true;
        }
        else{
            return false;
        }
    }
}


