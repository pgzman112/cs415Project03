//
// Created by Preston on 4/16/2021.
//
#include "Trie.h"
#include "TernarySearchTree.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <string>
#include <cctype>

using namespace std;

// I want to test searching for a string that is a whole word AND a predicate to another word.
// It should show the word and any auto completes that it can.

int main(int argc, char *argv[]){
    if(argc != 3){
        cout << "incorrect number of arguments try running again" << endl;
        exit(1);
    }
    string fileName = argv[1];
    string line = "";
    ifstream file(fileName);
    char * mode = argv[1];
    vector<string> inputText;
    if(file){
        // Loop through the input text file using getLine
        while(getline(file, line)){
            // Create the tempWord
            string tempWord;
            // Loop through entire line you just grabbed and add individual words
            // to our vector, this will also get rid of any punctuation
            for(int i = 0; i < line.length(); i++){
                if(isalpha(line[i])){
                    //cout << line[i] << endl;
                    char temp = line[i];
                    tempWord += (char) tolower(temp);
                }
                else{
                    inputText.push_back(tempWord);
                    tempWord = "";
                }
            }
            if(!tempWord.empty()){
                inputText.push_back(tempWord);
                tempWord = "";
            }
        }
        file.close();
    }
    else{
        cout << "Bad file input name please run program and try again." << endl;
    }
    Trie * trieRoot = new Trie();
    TernarySearchTree * ternaryRoot = new TernarySearchTree();
    for(auto & i : inputText){
       //cout << i << endl;
       trieRoot->insert(i);
        //cout << "did we make it?" << endl;
        int index = 0;
       ternaryRoot->insert(ternaryRoot->root, i, index);
        //cout << "did we make it?" << endl;
    }
   // cout << "did we make it?" << endl;
    // This is kind of all test stuff
    int lvl = 0;
    char str[100];
    trieRoot->print(trieRoot->root, str, lvl);
    vector<string> currTrie = trieRoot->printed;
    trieRoot->printed.clear();
    for(int i = 0; i < currTrie.size(); i++){
        cout << currTrie[i] << endl;
    }
   // trieRoot->search("w", true);
   // vector<string> tt = trieRoot->printed;
   // trieRoot->printed.clear();
   // for(auto i: tt){
   //     cout << "w" << i << endl;
   // }
   cout << " WHAT THE FUCK DOES THIS WORK???" << endl;
   ternaryRoot->inOrder();
   string find = "works";
   cout << endl;
   bool temp = ternaryRoot->search(ternaryRoot->root, find, 0);
   if(temp){
       cout << "found string: " << find << endl;
   }
   else{
       cout << "didnt find string: " << find << endl;
   }




    return 0;
}
