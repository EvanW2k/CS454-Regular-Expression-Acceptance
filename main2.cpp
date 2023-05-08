/* 
 * main.cpp
 * 
 * CS-454 Final Project
 * Evan Walters, Sean Sponsler
 * Date: May 15th, 2023
 * Description: Takes input of a Regular Expression as well as a string.
 *  Determnines if the string is accepted by the Regualar Expression
 * 
 */

#include <iostream>
#include <string>
#include <stack>
#include "NFA.hpp"
using namespace std;

bool isOp(char c);
int prio(char c);
string infixToPrefix(string RE);
/*
    2 inputs----------
        1) RE as string, with () > + > . > *
        2) String w to test acceptance

    Three steps are involved in solving this problem:
        1. convert the regular expression to an E-NFA
            - first create prefix string of RE
            - than create and combine NFA objects through prefix string
        2. remove E-moves 
        3. Test if w is accepted by the E-free NFA.
*/

int main() {
    /*
    string RE = "(A+B.C)*";
    string prefix = infixToPrefix(RE);
    cout << prefix << endl;
    */


    // ((a+b).d)*
    NFA M1('a');
    NFA M2('b');
    NFA M3(M1, '+', M2);
    //M3.print();
    //M3.removeEpsilon();
    //M3.print();
    NFA M4('d');
    NFA result(M3, '.', M4);
    NFA kleene(result, '*');
    return 0;
}

bool isOp(char c) {
    if (c == '+' || c == '.' || c == '*')
        return true;
    return false;
}

int prio(char c) {
    if (c == '*')
        return 3;
    else if (c == '.')
        return 2;
    else if (c == '+')
        return 1;
    return -1;
}

string infixToPrefix(string RE) {

    stack <char> s;
    string output = "";

    // reverse string
    reverse(RE.begin(), RE.end());

    // replace '(' with ')' and vica versa
    for (int i = 0; i < RE.length(); i++) {
        if (RE[i] == '(')
            RE[i] = ')';
        else if (RE[i] == ')')
            RE[i] = '(';
    }

    // iterate over reveresed RE
    for (int i = 0; i < RE.length(); i++) {

        // put symbols onto output string
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
