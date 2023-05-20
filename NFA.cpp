/*
 * File: NFA.cpp
 * 
 * Modified by: Sean Sponsler and Evan Waletrs
 * Date: May 20th, 2023
 * DESC: This file contians the implementation of a class named NFA, as well as
 *       the implementation of other functions for utility/main usage.
 */

#include "NFA.hpp"
using namespace std;


//-------------------------------------------------------------------
// FUNCTIONS FOR NFA CLASS                                          |
//-------------------------------------------------------------------

//********************************************************************************
// Function: simple NFA parameterized constructor
// In: a single character operand
// Return: NFA object created
// DESC: Given a sinlge operand a 2 state NFA is created (start = 0, final = 1).
//       The _delta is given data for 2 states, and a single transition between 
//       them using the operand.
//********************************************************************************
NFA::NFA(char operand) {

    addFinalState(1);  // for single operand constructor state 1 will be the final state
    _startState = 0;

    _delta.resize(2, vector<pair<int, char>> {});
    _delta[0].push_back(pair<int, char> {1, operand});


    _size = _delta.size();  // 2

}

//********************************************************************************
// Function: NFA parameterized constructor
// In: NFA, operator (+,.,*), NFA (not used with *)
// Return: NFA object created
// DESC: Creates an NFA object based on the given parameters.
//       In the case of union or concatenation. A new NFA is made my combinging 
//       them in the specified way. In case of kleene star only one NFA parameter 
//       is wanted, and the new NFA is made to accept repeating strings, and 
//       empty strings.
//******************************************************************************** 
NFA::NFA(NFA M1, char op, NFA M2) {     // M2 has a default value to make it an optional parameter
    assert(isOp(op));

    // UNION
    if (op == '+') {
        // start new NFA with 1 state then copy an incremented M1 to delta, and finals
        _delta.resize(1, vector<pair<int, char>> {});
        _size = 1;
        _startState = 0;

        for (int i = 0; i < M1.getSize(); i++) {
            vector<pair<int, char>> temp = M1.getDelta()[i];    // get first vector of pairs from M1

            // increment each state num in vector of pairs by 1
            for (int j = 0; j < temp.size(); j++) {
                get<0>(temp[j])++;
            }

            _delta.push_back(temp);     // push modified vector of M2 pairs 
            _size++;    // increment size
        }

        // copy over incremented finals from M1
        for (int i = 0; i < M1.getFinalStates().size(); i++) {
            addFinalState(M1.getFinalStates()[i] + 1);
        }

        // add transition from new start state to old M1 start state
        _delta[0].push_back({ M1.getStart() + 1, '&' });

        // Now concatenate M2 into new NFA with adjusted states
        for (int i = 0; i < M2.getSize(); i++) {
            vector<pair<int, char>> temp = M2.getDelta()[i];    // get first vector of pairs from M2

            // increment each state num in vector of pairs by size of M1
            for (int j = 0; j < temp.size(); j++) {
                get<0>(temp[j]) += M1.getSize() + 1;    // +1 for new start state
            }

            _delta.push_back(temp);     // push modified vector of M2 pairs 
            _size++;    // increment size
        }

        // copy over incremented finals from M2
        for (int i = 0; i < M2.getFinalStates().size(); i++) {
            addFinalState(M2.getFinalStates()[i] + M1.getSize() + 1);
        }

        // add transition from new start state to old M2 start state
        _delta[0].push_back({ M2.getStart() + M1.getSize() + 1, '&' });


    }

    // CONCATENATION
    else if (op == '.') {
        // copy M1 start, delta, and size to new NFA
        _size = M1.getSize();
        _delta = M1.getDelta();
        _startState = M1.getStart();

        // copy M2 into new NFA with adjusted states
        for (int i = 0; i < M2.getSize(); i++) {
            vector<pair<int, char>> temp = M2.getDelta()[i];    // get first vector of pairs from M2

            // increment each state num in vector of pairs by size of M1
            for (int j = 0; j < temp.size(); j++) {
                get<0>(temp[j]) += M1.getSize();
            }

            _delta.push_back(temp);     // push modified vector of M2 pairs 
            _size++;    // increment size
        }

        // add epsilon transitions from final states of M1 to start state of M2
        for (int i = 0; i < M1.getFinalStates().size(); i++) {
            // add pair at M2 start state as it is represented in new NFA
            _delta[M1.getFinalStates()[i]].push_back({ M2.getStart() + M1.getSize(), '&' });
        }

        // make current final states the same as M2 final states + M1.size
        for (int i = 0; i < M2.getFinalStates().size(); i++) {
            addFinalState(M2.getFinalStates()[i] + M1.getSize());
        }

    }

    // KLEEN STAR
    else if (op == '*') {
        // NOTE: This only modifies a copy of NFA M1 and doesn't use M2

        // copy M1 start, delta, finals, and size to new NFA
        _size = M1.getSize();
        _delta = M1.getDelta();
        _startState = M1.getStart();
        _finalStates = M1.getFinalStates();

        // allow for empty string by mapping start state to each final state with an epsilon transition
        for (int i = 0; i < M1.getFinalStates().size(); i++) {
            // allow for empty string by mapping start state to each final state with an epsilon transition
            _delta[_startState].push_back({ M1.getFinalStates()[i], '&' });
            // allow a loop of the NFA by mapping final states to start state 
            _delta[M1.getFinalStates()[i]].push_back({ _startState, '&' });
        }
    }

    // remove Epsilons fronm newly made NFA
    removeEpsilon();
}

//********************************************************************************
// Function: removeEpsilon
// In: none
// Return: epsilon free NFA
// DESC: changes the NFA to have zero epsilon transitions
//********************************************************************************
void NFA::removeEpsilon() {

    for (int i = 0; i < _delta.size(); i++) {
        for (int j = _delta[i].size() - 1; j >= 0; j--) {
            bool restart = false;   // used if a new & transition is added

            //if state i has transition to j via epsilon
            if (get<1>(_delta[i][j]) == '&') {
                //source = state = i
                //initalize destination
                int dst = get<0>(_delta[i][j]);

                _delta[i].erase(_delta[i].begin() + j); // delete & transition

                // add all transitions from source to all possible states reached from destination
                for (int k = 0; k < _delta[dst].size(); k++) {
                    
                    // don't push if the transition has a resulting state equal to source and an epsilon transition 
                    if (!(get<0>(_delta[dst][k]) == i && get<1>(_delta[dst][k]) == '&')) {
                        _delta[i].push_back(_delta[dst][k]);

                        // in case an & transition was added
                        if (get<1>(_delta[dst][k]) == '&') {
                            restart = true;
                        }
                    }     
                }
                // reset j
                if (restart) {
                    j = _delta[dst].size();
                }
                // in case destination if a final state make source state a final state
                if (isFinal(dst) && !isFinal(i)) {
                    addFinalState(i);
                }
            }
        }
    }
}

//********************************************************************************
// Function: bfs (breadth fist search)
// In: a string to test acceptance with the NFA
// Return: true/false
// DESC: Does a BFS to determine if the given string can be accepted by the NFA.
//********************************************************************************
bool NFA::bfs(string input) {
    // case '&' is used for string
    if (input == "&")
        input = "";

    // make a queue of pairs with the pair being (state number, string)
    queue<pair<int, string>> q;
    q.push(make_pair(_startState, input));

    while (!q.empty()) {
        // get values from front pair then pop
        int currState = get<0>(q.front());
        string currStr = get<1>(q.front());
        q.pop();

        // check for true scenario
        if (isFinal(currState) && currStr.empty())
            return true;

        // iterate for each transition at the current state
        for (int i = 0; i < _delta[currState].size(); i++) {
            // if there is a valid transition add it to queue
            if (get<1>(_delta[currState][i]) == currStr[0]) {
                q.push(make_pair(get<0>(_delta[currState][i]), currStr.substr(1))); // here we take off the first letter of the string for the pair
            }
        }
    }
    return false;
}

//********************************************************************************
// Function: isFinal
// In: a number for a state
// Return: true/false 
// DESC: Does a binary search of _finalStates with the given state number. 
//       Returns true/false based on if the number is found. 
//********************************************************************************
bool NFA::isFinal(int state) {

    int left = 0;
    int right = _finalStates.size() - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (_finalStates[mid] == state)
            return true;
        else if (_finalStates[mid] < state)
            left = mid + 1;
        else
            right = mid - 1;
    }

    return false;
}

//********************************************************************************
// Function: addFinalState
// In: a state number
// Return: none
// DESC: Used to add final states to vector to keep them in ascending order
//********************************************************************************
void NFA::addFinalState(int state) {
    if (!_finalStates.empty()) {
        // uses lower_bound form #include<algorithm>
        vector<int>::iterator it = lower_bound(_finalStates.begin(), _finalStates.end(), state);
        _finalStates.insert(it, state);
    }
    else
        _finalStates.push_back(state);

}

//********************************************************************************
// Function: print (used for testing)
// In: none
// Return: prints the NFA
// DESC: Prints the transitions at each state and a list of the final states
//********************************************************************************
void NFA::print() {
    for (int i = 0; i < _delta.size(); i++) {
        cout << "From state " << i << ": ";
        for (int j = 0; j < _delta[i].size(); j++) {
            cout << "(" << get<0>(_delta[i][j]) << ", " << get<1>(_delta[i][j]) << ") ";
        }
        cout << endl;
    }

    cout << "Final States: ";
    for (int i = 0; i < _finalStates.size(); i++) {
        cout << _finalStates[i] << " ";
    }
    cout << endl;
}
//-------------------------------------------------------------------
// FUNCTIONS FOR UTILITY / MAIN USAGE                               |
//-------------------------------------------------------------------

//********************************************************************************
// Function: accepts
// In: a Regular expression, and a string to test acceptance
// Return: none
// DESC: Outputs a statement based on if the string w is accepted by the RE
//********************************************************************************
void accepts(string RE, string w) {
    string temp = w;
    string prefixRE = infixToPrefix(RE);
    NFA M = convert(prefixRE);
    bool accept = M.bfs(w);
    //M.print();

    if (accept == true) {
        cout << "The string: " << w << "\nIs ACCEPTED by the RE: " << RE << endl;
    }
    else {
        cout << "The string: " << w << "\nIs NOT ACCEPTED by the RE: " << RE << endl;
    }

}

//********************************************************************************
// Function: isOp
// In: a character
// Return: true/false
// DESC: Returns true of the char is an operator, otherwise false
//********************************************************************************
bool isOp(char C) {
    return (C == '+' || C == '.' || C == '*') ? true : false;
}

//********************************************************************************
// Function: prio
// In: operator C
// Return: a priority number
// DESC: Given an operator a corresponding priority is returned 
//********************************************************************************
int prio(char C)
{
    if (C == '*')
        return 3;
    else if (C == '.')
        return 2;
    else if (C == '+')
        return 1;
    return -1;
}

//********************************************************************************
// Function: infixToPrefix
// In: string in the form of an infix regular expression
// Return: a regualr expression in prefix notation
// DESC: Given an infix regualr expression a prefix regular expression is 
//       returned.
//********************************************************************************
string infixToPrefix(string RE) {
    stack <char> s;
    string output = "";

    for (int i = 0; i < RE.length(); i++) {
        //check for consecutive terminal symbols or alpha near paren
        //(abc) and a(b.c) 
        if ((isalpha(RE[i]) || isdigit(RE[i])) && (i+1) < RE.length()) {
            if ((isalpha(RE[i+1]) || isdigit(RE[i+1])) || RE[i+1] == '(') {
                RE.insert(i+1, ".");
                continue;
            }
        }
        //((a.b)c)
        else if (RE[i] == ')' && (i+1) < RE.length()) {
            if (isalpha(RE[i+1]) || isdigit(RE[i+1])) {
                RE.insert(i+1, ".");
                continue;
            }
        }
    }
    //cout << "Formatted RE after concat insertion: " << RE << std::endl;

    // reverse string
    // example: (A+B.C)* -> *(C.B+A)
    reverse(RE.begin(), RE.end());

    // replace '(' with ')' and vica versa
    // *)C.B+A(
    for (int i = 0; i < RE.length(); i++) {
        if (RE[i] == '(')
            RE[i] = ')';
        else if (RE[i] == ')')
            RE[i] = '(';
    }

    // iterate over reversed RE
    for (int i = 0; i < RE.length(); i++) {
        // case alpha or digit or &: put operands onto output string
            // example :output = CBA
        if (isalpha(RE[i]) || isdigit(RE[i]) || RE[i] == '&') {
            output += RE[i];
        }
        // case (: put ( on stack
        else if (RE[i] == '(') {
            s.push(RE[i]);
        }
        // case ): put everything from stack onto output until (
        else if (RE[i] == ')') {
            while ((s.top() != '(') && (!s.empty())) {
                output += s.top();
                s.pop();
            }
            if (s.top() == '(') {
                s.pop();
            }
        }
        // in case of operator
        else if (isOp(RE[i])) {

            // when stack is empty just push
            if (s.empty()) {
                s.push(RE[i]);
            }

            else {
                // put current symbol on stack if prio > top of stack
                if (prio(RE[i]) > prio(s.top())) {
                    s.push(RE[i]);
                }
                // pop stack onto output until * higher than prio on stack 
                else if ((prio(RE[i]) == prio(s.top())) && (RE[i] == '*')) {
                    while ((!s.empty()) && (prio(RE[i]) == prio(s.top())) && (RE[i] == '*')) {
                        output += s.top();
                        s.pop();
                    }
                    s.push(RE[i]);  // after word put * on stack
                }

                // if same put on stack
                else if (prio(RE[i]) == prio(s.top())) {
                    s.push(RE[i]);
                }

                // while top of stack has prio > current, pop onto output
                else {
                    while ((!s.empty()) && (prio(RE[i]) < prio(s.top()))) {
                        output += s.top();
                        s.pop();
                    }
                    s.push(RE[i]);  // after push current to stack
                }
            }
        }
    }

    // pop rest of stack onto output
    while (!s.empty()) {
        output += s.top();
        s.pop();
    }

    // reverse string to get prefix
    reverse(output.begin(), output.end());

    // example *+A.BC
    return output;
}

//********************************************************************************
// Function: convert
// In: a string of a prefixed regular expression
// Return: an NFA
// DESC: converts a prefixed regular expression to an NFA
//********************************************************************************
NFA convert(string w) {
    return get<0>(convertHelper(0, w));
}

//********************************************************************************
// Function: convertHelper
// In: Int for position in string w (base case = 0),  
//     A string of a prefixed regular expression
// Return: Pair (NFA, int)
//         An NFA to be further added to and modified, 
//         Updated position to be used in string w
// DESC: Using recursion, converts a prefixed regular expression to an NFA, by 
//       using the int i to traverse the prefixed string and create and add to 
//       an NFA.
//********************************************************************************
pair<NFA, int> convertHelper(int i, string w) {
    
    // operand case
    if (!isOp(w[i])) {
        return pair<NFA, int> {NFA(w[i]), i + 1}; // create NFA with operand, inc i
    }

    // save op
    char op = w[i];

    // binary operator case (+, .)
    if (op == '+' || op == '.') {

        // combine next 2 NFAs (NFAs can be created with just single operands)
        pair<NFA, int> p = convertHelper(i + 1, w);
        pair<NFA, int> q = convertHelper(get<1>(p), w);
        NFA M(get<0>(p), op, get<0>(q));
        return pair<NFA, int> {M, get<1>(q)};
    }
    // unary operator case (*)
    else {
        // get next NFA then create an NFA object with * and next NFA
        pair<NFA, int> p = convertHelper(i + 1, w);
        NFA M(get<0>(p), op);
        return pair<NFA, int> {M, get<1>(p)};
    }
}