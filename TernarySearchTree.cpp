//
// Created by Preston on 4/16/2021.
//
#include <iostream>
#include <string>
#include <vector>
using namespace std;
#include "TernarySearchTree.h"

TernarySearchTree::TernarySearchTree() {
   root = nullptr;
}

struct Noode * TernarySearchTree::create(char t) {
    Noode * r = new Noode;
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
            cout << buffer << endl;
        }
        traverseHelper(nod->down, buffer,lvl+1);
        traverseHelper(nod->right, buffer, lvl);
    }
}

bool TernarySearchTree::search(struct Noode * nod, string s, int idx){
    if(!nod){
        //cout << "in here idx:" << idx << endl;
        //cout << "in her?" << endl;
        //char buffer[1000];
        //traverseHelper(nod, buffer, 0);
        return false;
    }
    if(s[idx] < nod->c){
        return search(nod->left, s, idx);
    }
    else if(s[idx] > nod->c){
        return search(nod->right, s, idx);
    }
    else {
        // This was an attempt at auto completes.
       // if(idx == s.length()-1){
       //     cout << "in her?" << endl;
       //     char buffer[1000];
       //     traverseHelper(nod, buffer, idx);
       // }
        if(idx == s.length()){
            //cout << "in her?" << endl;
            //char buffer[1000];
            //traverseHelper(nod, buffer, idx);
            return nod->endOfString;
        }
        return search(nod->down, s, idx+1);
    }
}

