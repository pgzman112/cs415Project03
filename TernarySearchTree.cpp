//
// Created by Preston on 4/16/2021.
//
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>
#include "TernarySearchTree.h"

using namespace std;
// Default constructor that just sets the root to nullptr
TernarySearchTree::TernarySearchTree() {
   root = nullptr;
   autoCompleteTime = 0;
}

//Create new Noode function
// sets the node's data to a passed in char, pointers are set to null and endOfString set to false
// Then returns the newly created node.
struct Noode * TernarySearchTree::create(char & t){
    Noode * r = new Noode;
    r->c = t;
    r->endOfString = false;
    r->left = r->down = r->right = nullptr;
    return r;
}

// Recursive function to insert a new string into the TST.
// takes in a pointer to the current node, a reference to the string s that should be inserted and
// an idx (index) of where we are in the current string.
struct Noode * TernarySearchTree::insert(struct Noode * currRoot, string & s, int & idx) {
    // To make things easy lets grab the current char that we're viewing
    char temp = s[idx];
    // if tree is empty
    if(currRoot == nullptr){
        // Calling the create node internal function and setting our current node to it.
        currRoot = create(temp);
        // This means its the first node ever so we are going to set it to our root node of the TST class!
        // This enables us to access the root node later on to search or insert into the tree again.
        if(root == nullptr){
            root = currRoot;
        }
    }
    // Now we check in the current temp character is less than the current roots character.
    // if its less than we want to skip this node and traverse down the left side of the node.
    if(temp < currRoot->c){
        // Set current roots left pointer recursively by calling insert on the left pointer
        currRoot->left = insert(currRoot->left, s, idx);
    }
    // similarly if current temp char is greater than current node's character we want to traverse the right node
    // this follows the same logic as above
    else if(temp > currRoot->c){
        currRoot->right = insert(currRoot->right, s, idx);
    }
    // if its not strictly less than, and not strictly greater than it must be equal.
    // we only traverse down on equal characters but we need to check that the index is not
    // larger than the string length to avoid any out of bounds issues.
    // if idx is less than string length it means there are more characters to potentially insert.
    // so we traverse down and recursively call insert on the down pointer while incrementing idx
    // (allowing us to traverse our input string)
    else if(idx < s.length()){
        currRoot->down = insert(currRoot->down, s, idx = idx + 1);
    }
    // this only happens when idx > s.length() meaning we are at the end of our input string.
    // set this current node's endOfString to true to mark this as a complete string.
    else{
        // I am pretty sure I am checking '\0' == '\0' here since s at s.length() is out of strings length if s is 'hello'
        // should have length of 5, but index's 0->4 not 100% sure why I added this check here lol.
        if(currRoot->c == s[s.length()]) {
            currRoot->endOfString = true;
        }
    }
    return currRoot;
}

// Doing an inorder traversal to check that it is building correctly.
void TernarySearchTree::inOrder(){
    char buffer[1000];
    traverseHelper(root, buffer, 0);
}

//recursive function to do an inorder traversal, takes in a current node pointer, a char buffer array and a level which is the current
// in the buffer array.
void TernarySearchTree::traverseHelper(struct Noode * nod, char * buffer, int lvl){
    // if there is a current node.
    if(nod){
        // First call on the left child since, that will come first before down or right children.
        traverseHelper(nod->left, buffer, lvl);
        //Add current char into buffer at current place held level
        buffer[lvl] = nod->c;
        // if this is an end of string node we wnt to push this current string into our vector of strings that holds all strings in the tree.
        // *this vector is used internally but needs to be cleared by the calling driver program after each operation that uses it.*
        // *it is not an elegant solution.*
        if(nod->endOfString){
            buffer[lvl+1] = '\0';
            string str(buffer);
            if(count(autoCompletes.begin(), autoCompletes.end(), str)){
                // do nothing since it already contains this possible autocorrect response.
            }
            else{
                autoCompletes.emplace_back(str);
            }
        }
        // recursively traverse down the down child and right child node.
        traverseHelper(nod->down, buffer,lvl+1);
        traverseHelper(nod->right, buffer, lvl);
    }
}

// This is the search function that allows to search a predicate and gather possible auto-correct
// responses based on that predicate string.
// returns true when a particular string S is found and false when it is not.
bool TernarySearchTree::search(struct Noode * nod, string & s, int idx, char * buffer, int lvl){
   bool predFound = false;
   // checkign our current buffer against the string to decide if this satisfies as a predicate.
   // since this function traverses recursively and at the end of each string of nodes, there is an empty
   // node that just says the previous was a complete string. therefore we have to perform this check once lvl = s.length()
    for(int i = 0; i < lvl && lvl == s.length(); i++){
        if(buffer[i] == s[i]){
            predFound = true;
        }
        else{
            predFound = false;
        }
    }
    // If we've found a predicate we want to make sure that we do an inOrder traversal on the current node,
    // that way we grab all possible strings that stem from this predicate. (satisfying the auto-complete goal)
    if(predFound){
        // Timer to keep track of the inorder traverse time. this will be subtracted from the total time back in the driver to find
        // search time and auto complete time explicitly.
        auto t1 = chrono::high_resolution_clock::now();
        traverseHelper(nod, buffer, lvl);
        auto t2 = chrono::high_resolution_clock::now();
        // this neat little trick just casts our auto variable (i think its a special chrono var type at this point)
        // into a double in the correct precision of milliseconds
        chrono::duration<double, milli> autoCompleteTime1 = t2-t1;
        // Since we may end up here more than once we don't want to accidently overwrite the auto-complete find time.
        // and that solves this issue.
        if(autoCompleteTime == 0){
            autoCompleteTime = autoCompleteTime1.count();
        }
        else{
            autoCompleteTime += autoCompleteTime1.count();
        }
    }
    // if current node does not exist then we have not found the string.
    if(!nod){
        return false;
    }
    // recursively traverse left or right till we find the correct char to traverse down on.
    if(s[idx] < nod->c){
        return search(nod->left, s, idx, buffer, lvl);
    }
    else if(s[idx] > nod->c){
        return search(nod->right, s, idx, buffer, lvl);
    }
    // if it is not strictly less than or greater than it must be equal
    else {
        // if index == s.length() we're at the end of the string.
        // so we can simply return the current nodes bool value of endOfString
        // which is true if curr node is the end of a string, and false if not.
        if(idx == s.length()){
            return nod->endOfString;
        }
        // if idx != s length we are not at end of string so push current char into the end of our buffer
        // then recursively call search while traversing the down node also increasing level and index by 1
        // to check the next char in the string.
        buffer[lvl] = nod->c;
        return search(nod->down, s, idx+1, buffer, lvl+1);
    }
}

// So since I have a peanut brain I tried to make searching and autocomplete work in the same function (like I did for tries)
// but it turns out that It does not work as smoothly as it did with Tries since we need to traverse this TST recursively.
// so this node returns if a passed in string s is found inside the TST, false if not.
// It follows almost exactly the same logic as the function above just without the auto-complete parts.
bool TernarySearchTree::searchNoAutoComplete(struct Noode * nod, string & s, int idx){
    //if current node doesn't exist return false.
    if(!nod){
        return false;
    }
    // if current char is less than nodes current char we traverse its left child recursively.
    if(s[idx] < nod->c){
        return searchNoAutoComplete(nod->left, s, idx);
    }
    // if current char is greater than nodes current char we traverse its right child recursively.
    else if(s[idx] > nod->c){
        return searchNoAutoComplete(nod->right, s, idx);
    }
    // if not strictly less than or greater and it exists it must be equal.
    else {
        // if we're at the end of the passed in string all we have to do is return current nodes
        // bool value of end of string.
        if(idx == s.length()){
            return nod->endOfString;
        }
        // If we're not at the end of the string we recursively traverse down the node while incrementing index.
        return searchNoAutoComplete(nod->down, s, idx+1);
    }
}