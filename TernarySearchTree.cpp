//
// Created by Preston on 4/16/2021.
//
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>
using namespace std;
#include "TernarySearchTree.h"

TernarySearchTree::TernarySearchTree() {
   root = nullptr;
}

struct Noode * TernarySearchTree::create(char t) {
    auto * r = new Noode;
    r->c = t;
    r->endOfString = false;
    r->left = r->down = r->right = nullptr;
    return r;
}

struct Noode * TernarySearchTree::insert(struct Noode * currRoot, string & s, int & idx) {
    char temp = s[idx];
    // if tree is empty
    if(currRoot == nullptr){
        currRoot = create(temp);
        // This means its the first node ever so gunna set it to our new root noode!
        if(root == nullptr){
            root = currRoot;
        }
    }
    if(temp < currRoot->c){
        currRoot->left = insert(currRoot->left, s, idx);
    }
    else if(temp > currRoot->c){
        currRoot->right = insert(currRoot->right, s, idx);
    }
    else if(idx < s.length()){
        currRoot->down = insert(currRoot->down, s, idx = idx + 1);
    }
    else{
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

void TernarySearchTree::traverseHelper(struct Noode * nod, char * buffer, int lvl){
    if(nod){
        traverseHelper(nod->left, buffer, lvl);
        buffer[lvl] = nod->c;
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
        traverseHelper(nod->down, buffer,lvl+1);
        traverseHelper(nod->right, buffer, lvl);
    }
}

bool TernarySearchTree::search(struct Noode * nod, string s, int idx, char * buffer, int lvl){
   bool predFound = false;
    for(int i = 0; i < lvl && lvl == s.length(); i++){
        if(buffer[i] == s[i]){
            predFound = true;
        }
        else{
            predFound = false;
        }
    }
    if(predFound){
        auto t1 = chrono::high_resolution_clock::now();
        traverseHelper(nod, buffer, lvl);
        auto t2 = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> autoCompleteTime1 = t2-t1;
        if(autoCompleteTime == 0){
        //    cout << "Pushing: " << autoCompleteTime1.count() << endl;
            autoCompleteTime = autoCompleteTime1.count();
        }
        else{
          //  cout << "Pushing: " << autoCompleteTime1.count() << endl;
            autoCompleteTime += autoCompleteTime1.count();
            //cout << autoCompleteTime << endl;
        }
    }
    if(!nod){
        return false;
    }
    if(s[idx] < nod->c){
        return search(nod->left, s, idx, buffer, lvl);
    }
    else if(s[idx] > nod->c){
        return search(nod->right, s, idx, buffer, lvl);
    }
    else {
        if(idx == s.length()){
            return nod->endOfString;
        }
        buffer[lvl] = nod->c;
        return search(nod->down, s, idx+1, buffer, lvl+1);
    }
}

