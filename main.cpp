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
    NFA M1('a');
    NFA M2('b');
    NFA M3(M1, '+', M2);
    NFA M4('d');
    NFA result(M3, '.', M4);
    result.print();
    
   // result2.print();

    //cout << "test" << endl;
    //string RE = "x+y*z/w+u";


    //// get RE input
    //string RE = "";
    //bool readable = true;

    //// loop until
    //do {

    //    // for new user input
    //    if (!readable)
    //        cout << "Please renter your Regualr Expression. Remember the rules.\n";

    //    // prompt and rules
    //    cout << "Enter regular expression in a single line with these rules:\n"
    //        << "1. \"+\" = union, \".\" = concatination, \"*\" = kleene star, \"&\" = epsilon), \"(\" and \")\" are parenthesis\n"
    //        << "2. Must use \".\" wherever possible to distiunguish symbols, and symbols can only be lowercase w/ or w/o numbers\n\n"
    //        << "Enter Regular Expression: ";

    //    cin >> RE;

    //    //confirm readable RE
    //    for (int i = 0; i < RE.length(); i++) {
    //        if (RE[i] == '+' || RE[i] == '&') { //only allowed uppercase symbol
    //            continue;
    //        }
    //        else if (isalpha(RE[i])) {
    //            RE[i] = tolower(RE[i]);
    //        }
    //        /*
    //        if (RE[i] != '(' || RE[i] != ')' || RE[i] != '.' || RE[i] != '*') {
    //            cerr << "Invalid input. Allowed characters [a-z, A-Z, (), ., *, +]" << endl;
    //            exit(1);
    //        }
    //        */
    //    }

    //} while (!readable);


    //// get string to test in RE
    //string w = "";
    //cout << "Enter string w to be tested against given regular expression: " << endl;
    //cin >> w;
    ////confirm readable string
    //for (int i = 0; i < w.length(); i++) {
    //    if (isalpha(w[i])) {
    //        w[i] = tolower(w[i]);
    //    }
    //    else {
    //        cerr << "Invalid input. Allowed characters [a-z, A-Z]." << endl;
    //        exit(1);
    //    }
    //}

    return 0;
}
