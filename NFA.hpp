/*
 * File: NFA.hpp
 * 
 * Modified by: Sean Sponsler and Evan Waletrs
 * Date: May 20th, 2023
 * DESC: This file contians the description of a class named NFA, as well as
 *       other functions for utility/main usage.
 */

#include <iostream>
#include <vector>
#include <utility> // pair
#include <queue>
#include <stack>
#include <string>
#include <assert.h>
#include <algorithm>
#include "State.hpp"


class NFA {
public:

    // Constructors & Destructor
    NFA() {};
    NFA(char operand);
    NFA(NFA M1, char op, NFA M2 = NFA());
    ~NFA() {};

    void removeEpsilon();
    bool bfs(std::string input);

    bool isFinal(int state);
    void addFinalState(int state);

    // inline set functions
    void setFinalStates(std::vector<int> final) { _finalStates = final; } 
    void setDelta(std::vector<std::vector<std::pair<int, char>>> delta) { _delta = delta; } 

    // inline get functions
    int getSize() { return _size; }                      
    int getStart() { return _startState; }   
    std::vector<int> getFinalStates() { return _finalStates; }                        
    std::vector<std::vector<std::pair<int, char>>> getDelta() { return _delta; }

    // print function for testing
    void print();

private:
    std::vector<int> _finalStates;
    int _startState = 0;
    int _size = 0;
    // 2d vector of pairs (result state, transition operand)
    std::vector<std::vector<std::pair<int, char>>> _delta;

};


// Other functions for utility/main usage.

void accepts(std::string RE, std::string w);
bool isOp(char c);
int prio(char c);
std::string infixToPrefix(std::string RE);
NFA convert(std::string w);
std::pair<NFA, int> convertHelper(int i, std::string w);
