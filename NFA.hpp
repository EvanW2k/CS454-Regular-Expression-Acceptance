#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <assert.h>
#include <algorithm>
#include "State.hpp"


/*
    PERHAPS: delta is 2d vector holding pairs{current state, transition} as states
*/

class NFA {
public:
    NFA(char operand);
    NFA(std::vector<int> finalStates, std::vector<std::vector<char>> delta) : _finalStates{finalStates}, _delta{delta} {};
    
    //_delta = Union of M1 + M2
    NFA(NFA M1, NFA M2, char op);

    int getSize() { return _size; }
    std::vector<int> getFinalStates() { return _finalStates; }
    std::vector<std::vector<char>> getDelta() { return _delta; }
    void setFinalStates(std::vector<int> final) { _finalStates = final; }
    void setDelta(std::vector<std::vector<char>> delta) { _delta = delta; }

    //i is starting position of current index, RE is prefix string containing input symbols
    //recursive
    std::pair<NFA, int> convert(int i, std::string RE);

    void print();
private:
    std::vector<int> _finalStates;
    int _size = 0;
    //change to hashtable
    std::vector<std::vector<char>> _delta;

};
