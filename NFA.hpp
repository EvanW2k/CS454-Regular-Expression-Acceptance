#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <algorithm>
#include "State.hpp"


/*
    NFA holds delta function (2d vector of states [input] [state]) & final states
        what data structure to store 2d vector
        hashtable vs. 

    DO WE NEED STATE?
        if we need any more information about state yes
        if just boolean accepting, no
*/

class NFA {
public:
    NFA(std::string symbols);
    NFA(std::vector<State> finalStates, std::vector<std::vector<State>> delta) : _finalStates{finalStates}, _delta{delta} {};

    //_delta = Union of M1 + M2
    NFA combine(NFA M1, NFA M2);

    //i is starting position of current index, RE is prefix string containing input symbols
    //recursive
    std::pair<NFA, int> convert(int i, std::string RE);

    //void print();
private:
    std::vector<State> _finalStates;
    //change to hashtable
    std::vector<std::vector<State>> _delta;

};
