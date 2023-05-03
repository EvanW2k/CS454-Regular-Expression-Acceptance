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


NFA::NFA(char operand) {

    // & => epsilon / null
    // - => empty / no transition
    
    vector<char> states { '&', operand};
    vector<char> empty {'-', '-'};
    
    //delta holds 1 vector of 2 state NFA and an empty vector
    _delta.push_back(states);
    _delta.push_back(empty);

    //1 is operand in <states>, transition to final
    _finalStates.push_back(1);
    _size = 2;
}

// 
NFA::NFA(NFA M1, NFA M2, char op) {  
    assert(isOp(op));

    _size = M1._size + M2._size;
    if (op == '+') {
        
    } 
    else if (op == '.') {
        //vector1.insert( vector1.end(), vector2.begin(), vector2.end() );
        //length of nfa 1 in empty spaces before 
        vector<vector<char>> delta = M1.getDelta();
        for (int i = 0; i < delta.size(); i++) {
            delta[i].resize(_size, '-');
        }
        delta.resize(_size, vector<char>(_size, '-'));
        vector<int> M1F = M1.getFinalStates();
        vector<int> M2F = M2.getFinalStates();
        for (int i = 0; i < M1F.size(); i++) {
            //change final states of M1 into epsilon transitions to starting states of M2
            delta[M1F[i]][M1._size] = '&';
        }
        //think further
        for (int i = 0; i < M2F.size(); i++) {
            delta[M1._size][M2F[i] + M1._size];
        }
    } 
    else if (op == '*') {

    } 
}

void NFA::print() {
    cout << "Printing delta function contents of NFA: " << endl;
    for (int i = 0; i < _delta.size(); i++) {
        for (int j = 0; j < _delta[i].size(); j++) {
            if (_delta[i][j] != '&') {
                cout << "delta[" << i << "][" << j << "] = " << _delta[i][j] << " ";
            }
        }
        cout << endl;
    }
}

int main() {
    /*
    string RE = "(A+B.C)*";
    string prefix = infixToPrefix(RE);
    cout << prefix << endl;
    */
    NFA M1('a');
    M1.print();


    return 0;
}