#include "NFA.hpp"
using namespace std;

bool isOp(char C) {
    return (C == '+' || C == '.' || C == '*') ? true : false;
}

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

string infixToPrefix(string RE) {
    stack <char> s;
    string output = "";

    // reverse string
    // (A+B.C)* -> *(C.B+A)
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

        // put symbols onto output string
        // output = CBA
        if (isalpha(RE[i]) || isdigit(RE[i])) {
            output += RE[i];
        }
        // put ( on stack
        else if (RE[i] == '(') {
            s.push(RE[i]);
        }
        // put everything from stack onto output until (
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

    return output;
}

NFA::NFA() {


}

NFA::NFA(char operand) {

    _finalStates.push_back(1);  // for single operand constructor state 1 will be the final state
    _startState = 0;

   _delta.resize(2, vector<pair<int, char>> {});
   _delta[0].push_back(pair<int, char> {1, operand});


    _size = _delta.size();  // 2
  
}

// 
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
            _finalStates.push_back(M1.getFinalStates()[i] + 1);
        }

        // add transition from new start state to old M1 start state
        _delta[0].push_back({ M1.getStart()+1, '&'});

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
            _finalStates.push_back(M2.getFinalStates()[i] + M1.getSize() + 1);
        }

        // add transition from new start state to old M2 start state
        _delta[0].push_back({M2.getStart() + M1.getSize() + 1, '&'});


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
            _delta[M1.getFinalStates()[i]].push_back({M2.getStart() + M1.getSize(), '&'});
        }

        // make current final states the same as M2 final states + M1.size
        for (int i = 0; i < M2.getFinalStates().size(); i++) {
            _finalStates.push_back(M2.getFinalStates()[i] + M1.getSize());
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
            _delta[_startState].push_back({M1.getFinalStates()[i], '&'});
            // allow a loop of the NFA by mapping final states to start state 
            _delta[M1.getFinalStates()[i]].push_back({_startState, '&'});
        }
    }
    removeEpsilon();
}
/*
*/
void NFA::removeEpsilon() {
    for (int i = 0; i < _delta.size(); i++) {
        for (int j = _delta[i].size() - 1; j >= 0; j--) {
            bool restart = false;
            //if state i has transition to j via epsilon
            if (get<1>(_delta[i][j]) == '&') {
                //source = state = i
                //destination = 
                int dst = get<0>(_delta[i][j]);

                _delta[i].erase(_delta[i].begin() + j);
                for (int k = 0; k < _delta[dst].size(); k++) {
                    //transition from source to all possible states reached from destination
                    _delta[i].push_back(_delta[dst][k]);

                    //CAN BE OPTIMIZED
                    if (get<1>(_delta[dst][k]) == '&') {
                        cout << "restarting at " << dst << ", " << k << endl;
                        restart = true;
                    }
                }
                if (restart) {
                    j = _delta[dst].size();
                }
                if (isFinal(dst)) {
                    _finalStates.push_back(i);
                }
            }
        }
    }
}

/*
*/
bool NFA::isFinal(int state) {
    for (int i = 0; i < _finalStates.size(); i++) {
        if (_finalStates[i] == state) return true;
    }
    return false;
}

/*
*/





void NFA::print() {
    for (int i = 0; i < _delta.size(); i++) {
        cout << "From state " << i << ": ";
        for (int j = 0; j < _delta[i].size(); j++) {
            cout << "(" << get<0>(_delta[i][j]) << ", " << get<1>(_delta[i][j]) << ") ";
        }
        cout << endl;
    }
}
