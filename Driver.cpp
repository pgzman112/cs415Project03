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
// It should show the word and any auto completes that it can. XX DONE.

// This is used to find how much space each data structure uses.
long get_mem_usage(){
    struct rusage myusage;
    getrusage(RUSAGE_SELF, &myusage);
    return myusage.ru_maxrss;
}

int main(int argc, char *argv[]){
    // Check if correct amount of arguments are passed, exit if they arent.
    if(argc != 3){
        cout << "incorrect number of arguments try running again" << endl;
        exit(1);
    }
    string fileName = argv[1];
    string line = "";
    ifstream file(fileName);
    char * mode = argv[2];
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
        exit(1);
    }
    // Get baseline memory usage before building Trie data structure.
    long baseline = get_mem_usage();
    // create a new Trie
    Trie * trieRoot = new Trie();
    // start timer
    auto t5 = chrono::high_resolution_clock::now();
    // insert each word from the input text into the Trie
    for(auto & i : inputText){
       trieRoot->insert(i);
    }
    auto t6 = chrono::high_resolution_clock::now();
    // time.end - time.start gives us runtime.
    chrono::duration<double, milli> trieBuildTime = t6-t5;
    double trieBuild = trieBuildTime.count();

    // Check memory used once Trie has been built.
    // The memory used by the trie alone should roughly be the afterTrie - baseline.
    long afterTrie = get_mem_usage();
    // Storing Trie and base together to later subtract from TST to get a rough memory usage of it.
    long trieAndBase = afterTrie;
    afterTrie = afterTrie - baseline;
    // Create a new TST
    TernarySearchTree * ternaryRoot = new TernarySearchTree();
    // Set up a timer to capture how long it takes to insert the input Text into the TST
    auto t7 = chrono::high_resolution_clock::now();
    for(auto & i : inputText){
        int index = 0;
        ternaryRoot->insert(ternaryRoot->root, i, index);
    }
    auto t8 = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> ternBuildTime = t8-t7;
    double ternaryBuild = ternBuildTime.count();
    // get total program memory usage after building TST then subtract the total mem usage of baseline + Trie structure.
    long afterTernary = get_mem_usage();
    afterTernary = afterTernary - trieAndBase;

    // if mode entered is 1 we go into here!
    if(mode[0] == '1'){
        // Print out time to build each structure and space they occupy.
        cout << "Time taken to build the standard Trie is: " << trieBuild << " ms and space occupied by it is: " << afterTrie << " bytes." << endl;
        cout << "Time taken to build the BST based Trie is: " << ternaryBuild << " ms and space occupied by it is: " << afterTernary << " bytes." << endl << endl;
        bool run = true;
        // Runs until user hits ctrl c  or cmd c since we want the user to be able to search any string such as quit
        // q, exit etc.
        while(run){
            // Making sure the autoComplete timer is reset in our TST.
            trieRoot->autoCompleteTime = 0;
            string input = "";
            // Have user enter a string or predicate they would liek to search for.
            cout << "Enter search string (or hit 'ctrl + c'/ 'cmd + c' to quit program): ";
            cin >> input;
            bool searchie = true;
            // time the total time it takes to search and find possible autcompletes from the Trie data structure.
            auto t3 = chrono::high_resolution_clock::now();
            bool searchTrie = trieRoot->search(input, searchie);
            auto t4 = chrono::high_resolution_clock::now();
            chrono::duration<double, milli> totalTimeTrie = t4-t3;
            // Subtract the time it took to find the auto-complete solutions to get total search time.
            double trieSearchTime = totalTimeTrie.count() - trieRoot->autoCompleteTime;
            cout << "Time taken to search in the standard Trie is: " << trieSearchTime << " ms." << endl;
            // If it found what we're looking for lets print it out.
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
            // Reset the internal timer holder for the next search
            trieRoot->autoCompleteTime = 0;

            // Repeate the same process searching for input string as well as auto complete answer in the TST
            char buff[1000];
            int lvl = 0;
            // Time total search and auto-complete solution time.
            auto t1 = chrono::high_resolution_clock::now();
            ternaryRoot->search(ternaryRoot->root, input, 0, buff, lvl);
            auto t2 = chrono::high_resolution_clock::now();
            chrono::duration<double, milli> totalTime = t2-t1;
            double searchTime = 0;
            // Subtract auto-complete time to get total search time by itself.
            searchTime = totalTime.count() - ternaryRoot->autoCompleteTime;
            cout << "Time taken to search in the BST based Trie is: " << searchTime << " ms." <<endl;
            // If autoCompletes is empty it means string was not found and no-possible autocompletes exist
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
            }
        } // END run
    }
    // Mode 2 means we're just going to search for all possible input strings, in both the Trie and TST and compare the times.
    // This Trie should obviously be MUCH faster at searching them all sincee that is its advantage. While the TST is slower it
    // Uses MUCH less space
    else if(mode[0] == '2'){
        cout << "Time taken to build the standard Trie is: " << trieBuild << " ms and space occupied by it is: " << afterTrie << " bytes." << endl;
        cout << "Time taken to build the BST based Trie is: " << ternaryBuild << " ms and space occupied by it is: " << afterTernary << " bytes." << endl << endl;

        // Search for all strings within the trie.
        auto t1 = chrono::high_resolution_clock::now();
        bool searchie = false;
        for(int i = 0; i < inputText.size(); i++){
            bool searchTrie = trieRoot->search(inputText[i], searchie);
            if(searchTrie == false){
                // In case we don't find a string!
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
            ternaryRoot->autoCompletes.clear();
            ternaryRoot->searchNoAutoComplete(ternaryRoot->root, inputText[i], 0);
            ternaryRoot->autoCompleteTime = 0;
        }
        auto t4 = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> tstSearchTime = t4-t3;
        double searchTime = 0;
        searchTime = tstSearchTime.count();
        cout << "Time taken to search all the strings in the Ternary search tree is: " << searchTime << " ms." << endl;
    }
    // If anything besides 1 or 2 is entered for mode it exists.
    else{
        cout << "wrong mode entry, please run again and input '1' or '2' for mode." << endl;
        exit(1);
    }
    return 0;
}
