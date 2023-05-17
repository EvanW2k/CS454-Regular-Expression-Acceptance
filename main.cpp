/*
 * File: main.cpp
 *
 * CS-454 Final Project
 * Modified by: Evan Walters, Sean Sponsler
 * Date: May 15th, 2023
 * DESC: This program takes input of a Regular Expression as well as a string
 *       and Determnines if the string is accepted by the Regualar Expression.
 *
 */


#include <iostream>
#include <string>
#include "NFA.hpp"
using namespace std;


// Sean, make sure there is a loop so the user can enter as many as they would like, make it stop when % or something is used as the RE

int main() {

    //accepts("((a+b).d)*", "adadadadbdbdadadadadadadadadbdbdbdbdbdbdbdbdbdbdbdbd");
    accepts("(a+b.c)*", "&");
    //accepts("(a)*", "a");


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
