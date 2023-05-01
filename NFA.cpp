#include "NFA.hpp"
using namespace std;

bool isOperator(char C) {
    return (C == '+' || C == '.' || C == '*') ? true : false;
}

int precedence(char C)
{
    if (C == '*')
        return 1;
    else if (C == '.')
        return 2;
    else if (C == '+')
        return 3;
    return -1;
}

string infixToPrefix(string infix) {
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
                else if ((prio(RE[i]) >= prio(s.top())) && (RE[i] == '*')) {
                    while ((prio(RE[i]) >= prio(s.top())) && (RE[i] == '*')) {
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
