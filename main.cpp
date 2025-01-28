/*
 * File: main.cpp
 *
 * CS-454 Final Project
 * Modified by: Evan Walters, Sean Sponsler
 * Date: May 15th, 2023
 * DESC: This program takes input of a Regular Expression as well as a string
 *       and Determines if the string is accepted by the Regular Expression.
 *
 */


#include <iostream>
#include <string>
#include "NFA.hpp"
using namespace std;



int main() {

    //accepts("((a+b).d)*", "adadadadbdbdadadadadadadadadbdbdbdbdbdbdbdbdbdbdbdbd");
    //accepts("(a+b.c)*", "&");
    //accepts("(a)*", "a");

    //// get RE input
    string RE = "";
    string w = "";
    
    bool readable = true;
    

    //// loop until
    do {

        // for new user input
        if (!readable)
            cout << "Regular expression is not readable. \n";

        // prompt and rules
        cout << "-------------------------------------------------------" << endl;
        cout << "Enter regular expression in a single line with these rules:\n"
            << "1. \"+\" = union, \".\" = concatenation, \"*\" = kleene star, \"&\" = epsilon/null, \"(\" and \")\" are parentheses\n"
            << "2. Symbols are single characters, can only be upper/lowercase letters or numbers (\"a0\" will be interpreted as \"a.0\")\n"
            << "3. Enter % by itself to quit the program.\n\n"
            << "Enter Regular Expression(% to quit): ";

        cin >> RE;
        if (RE == "%") {
            return 0;
        }

        //confirm readable RE
        for (int i = 0; i < RE.length(); i++) {        
            if (!isalpha(RE[i]) && !isdigit(RE[i]) && RE[i] != '(' && RE[i] != ')' && RE[i] != '.' && RE[i] != '*' && RE[i] != '+' && RE[i] != '&') {
                cerr << "Invalid input. Allowed characters [a-z, A-Z, 0-9, (), ., *, +, &]. Invalid character: " << RE[i] <<  "\n" << endl;
                exit(1);
            }
        }

        cout << "Enter string w to be tested for acceptance against given Regular Expression using only alphanumeric characters and epsilon(&)(% to quit): ";
        cin >> w;
        if (w == "%") {
            return 0;
        }

        bool null = false;
        if (w.find("&") != string::npos) {  // determine if there is an epsilon in the string
            null = true;
            // (&)* => &
            for (int i = 0; i < w.length(); i++) {
                if (w[i] == '&') {
                    continue;
                } else {
                    null = false;
                    // erase all epsilon (&)
                    w.erase(remove(w.begin(), w.end(), '&'), w.end());
                }
            }
        }
        // in case the entire string w is some form of &
        if (null) {
            cout << "\n\n";
            accepts(RE, "&");
        }
        else {
            // check w for invalid characters
            for (int i = 0; i < w.length(); i++) {
                cout << w[i] << endl;
                if (!isalpha(w[i]) && !isdigit(w[i]) && w[i] != '&') {
                    cerr << "Invalid input. Allowed characters [a-z, A-Z, 0-9] \n" << endl;
                    exit(1);
                }
            }
            cout << "\n\n";
            accepts(RE, w); // test acceptance by RE
        }

    } while (readable);


    return 0;
}
