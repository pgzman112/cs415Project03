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
#include <chrono>
#include <sys/time.h>
#include <sys/resource.h>

using namespace std;

// I want to test searching for a string that is a whole word AND a predicate to another word.
// It should show the word and any auto completes that it can.
long get_mem_usage(){
    struct rusage myusage;
    getrusage(RUSAGE_SELF, &myusage);
    return myusage.ru_maxrss;
}

int main(int argc, char *argv[]){
    if(argc != 3){
        cout << "incorrect number of arguments try running again" << endl;
        exit(1);
    }
    string fileName = argv[1];
    string line = "";
    ifstream file(fileName);
    char * mode = argv[2];
    vector<string> inputText;
    long baseline = get_mem_usage();
    //cout << "BASELINE: " << baseline << endl;
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
        exit(1);
    }
    Trie * trieRoot = new Trie();
    TernarySearchTree * ternaryRoot = new TernarySearchTree();
    auto t5 = chrono::high_resolution_clock::now();
    for(auto & i : inputText){
       trieRoot->insert(i);
    }
    auto t6 = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> trieBuildTime = t6-t5;
    double trieBuild = trieBuildTime.count();
   // cout << "baseLine: " << baseline << endl;
    long afterTrie = get_mem_usage();
    long trieAndBase = afterTrie;
    //cout << "afterTrie: " << afterTrie << endl;
    afterTrie = afterTrie - baseline;
    auto t7 = chrono::high_resolution_clock::now();
    for(auto & i : inputText){
        int index = 0;
        ternaryRoot->insert(ternaryRoot->root, i, index);
    }
    auto t8 = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> ternBuildTime = t8-t7;
    double ternaryBuild = ternBuildTime.count();
    long afterTernary = get_mem_usage();
   // cout << "afterTern: " << afterTernary << endl;
    afterTernary = afterTernary - trieAndBase;
    //trieRoot->search("temp", false);
    //cout << "TernarySize: " << afterTernary-afterTrie << endl;
   // cout << "did we make it?" << endl;
    // This is kind of all test stuff

   //int lvl = 0;
   //char str[100];
   //trieRoot->print(trieRoot->root, str, lvl);
   //vector<string> currTrie = trieRoot->printed;
   //trieRoot->printed.clear();
   //for(int i = 0; i < currTrie.size(); i++){
   //    cout << currTrie[i] << endl;
   //}

   // trieRoot->search("w", true);
   // vector<string> tt = trieRoot->printed;
   // trieRoot->printed.clear();
   // for(auto i: tt){
   //     cout << "w" << i << endl;
   // }
   //ternaryRoot->inOrder();
//string find = "taband";
/// cout << endl;
//char buff[1000];
//int lvl=0;
////string buff;
//bool temp = ternaryRoot->search(ternaryRoot->root, find, 0, buff, lvl);
//if(temp){
//    cout << "found string: " << find << endl;
//}
//else{
//    cout << "didnt find string: " << find << endl;
//}
//cout << "**********" << endl;
// for(auto i = 0; i < ternaryRoot->autoCompletes.size(); i++){
//     cout << ternaryRoot->autoCompletes[i] << endl;
// }
// cout << "**********" << endl;
// ternaryRoot->autoCompletes.clear();
//  for(auto i = 0; i < ternaryRoot->autoCompletes.size(); i++){
//      cout << ternaryRoot->autoCompletes[i] << endl;
//  }

  // cout << "size of trie tree: " << afterTrie << endl;
  // cout << "size of ternary tree: " << afterTernary<< endl;

    // The zeros are placeholders until I set up timers!

    if(mode[0] == '1'){
        cout << "Time taken to build the standard Trie is: " << trieBuild << " ms and space occupied by it is: " << afterTrie << " bytes." << endl;
        cout << "Time taken to build the BST based Trie is: " << ternaryBuild << " ms and space occupied by it is: " << afterTernary << " bytes." << endl << endl;
        bool run = true;
        while(run){
            trieRoot->autoCompleteTime = 0;
            string input = "";
            cout << "Enter search string (or hit 'ctrl + c'/ 'cmd + c' to quit program): ";
            cin >> input;
            bool searchie = true;
            auto t3 = chrono::high_resolution_clock::now();
            bool searchTrie = trieRoot->search(input, searchie);
            auto t4 = chrono::high_resolution_clock::now();
            chrono::duration<double, milli> totalTimeTrie = t4-t3;
            double trieSearchTime = totalTimeTrie.count() - trieRoot->autoCompleteTime;
            cout << "Time taken to search in the standard Trie is: " << trieSearchTime << " ms." << endl;
            if(searchTrie){
               vector<string> currTrie = trieRoot->printed;
               trieRoot->printed.clear();
               string first;
               if(input == currTrie[0]){
                   first = input;
               }
               else{
                    first = input + currTrie[0];
               }
               cout << "Auto-complete results using standard Trie are: " << first << ", ";
               for(int i = 1; i < currTrie.size(); i++){
                   cout << input << currTrie[i];
                   if(i < currTrie.size()-1){
                       cout << ", ";
                   }
               }
               cout << "." << endl;
               cout << "Time Taken to find auto-complete results in the standard Trie was: " << trieRoot->autoCompleteTime << " ms." << endl << endl;
            }
            else{
                cout << input << " not found in text and no autocompletes found" << endl;
            }
            trieRoot->autoCompleteTime = 0;

            char buff[1000];
            int lvl = 0;
            auto t1 = chrono::high_resolution_clock::now();
            ternaryRoot->search(ternaryRoot->root, input, 0, buff, lvl);
            auto t2 = chrono::high_resolution_clock::now();
            chrono::duration<double, milli> totalTime = t2-t1;
            double searchTime = 0;
            searchTime = totalTime.count() - ternaryRoot->autoCompleteTime;
            cout << "Time taken to search in the BST based Trie is: " << searchTime << " ms." <<endl;
            if(ternaryRoot->autoCompletes.empty()){
                cout << input << " not found in text and no autocompletes found" << endl;
            }
            else {
                cout << "Auto-complete results using BST based Trie are: " << ternaryRoot->autoCompletes[0] << ", ";
                for (auto i = 1; i < ternaryRoot->autoCompletes.size(); i++) {
                    cout << ternaryRoot->autoCompletes[i];
                    if (i < ternaryRoot->autoCompletes.size() - 1) {
                        cout << ", ";
                    }
                }
                cout << "." << endl;
                ternaryRoot->autoCompletes.clear();
                cout << "Time Taken to find auto-complete results in the BST based Trie is: "
                     << ternaryRoot->autoCompleteTime << " ms." << endl << endl;
                ternaryRoot->autoCompleteTime = 0;
                //cout << "Search time Delta: " << trieSearchTime - searchTime << endl << endl;
            }
        } // END run
    }
    else if(mode[0] == '2'){
        cout << "Time taken to build the standard Trie is: " << trieBuild << " ms and space occupied by it is: " << afterTrie << " bytes." << endl;
        cout << "Time taken to build the BST based Trie is: " << ternaryBuild << " ms and space occupied by it is: " << afterTernary << " bytes." << endl << endl;

        // Search for all strings within the trie.
        auto t1 = chrono::high_resolution_clock::now();
        bool searchie = false;
        for(int i = 0; i < inputText.size(); i++){
            bool searchTrie = trieRoot->search(inputText[i], searchie);
            if(searchTrie == false){
                cout << "UH OH NOT GOOD" << endl;
            }
        }
        auto t2 = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> totalTime = t2-t1;
        double searchTrieTime = 0;
        searchTrieTime = totalTime.count();
        cout << "Time taken to search all the strings in the standard Trie tree is: " << searchTrieTime << " ms." << endl;
        // Search for all strings within the TST

        auto t3 = chrono::high_resolution_clock::now();
        //double runningTot;
        for(int i = 0; i < inputText.size(); i++){
            //char buff[1000];
            //int lvl = 0;

            //cout << "what are we stuck on " << inputText[i] << endl;
            ternaryRoot->autoCompletes.clear();
            ternaryRoot->searchNoAutoComplete(ternaryRoot->root, inputText[i], 0);
            //runningTot += ternaryRoot->autoCompleteTime;
            ternaryRoot->autoCompleteTime = 0;
        }
        auto t4 = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> tstSearchTime = t4-t3;
        double searchTime = 0;
        searchTime = tstSearchTime.count();
        cout << "Time taken to search all the strings in the Ternary search tree is: " << searchTime << " ms." << endl;
        //cout << "after subtracting autocomplete time: " << searchTime - runningTot << endl;
    }
    else{
        cout << "wrong mode entry, please run again and input '1' or '2' for mode." << endl;
        exit(1);
    }



    return 0;
}
