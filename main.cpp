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
                //readable = false;
                //break;
                //exit(1);
            }
        }
        //cout << "\n";
        cout << "Enter string w to be tested for acceptance against given Regular Expression using only alphanumeric characters and epsilon(&)(% to quit): ";
        cin >> w;
        if (w == "%") {
            return 0;
        }
        //cout << "\n";
        bool null = false;
        if (w.find("&") != string::npos) {
            null = true;
            // (&)* => &
            for (int i = 0; i < w.length(); i++) {
                if (w[i] == '&') {
                    continue;
                } else {
                    null = false;
                    //erase all &
                    w.erase(remove(w.begin(), w.end(), '&'), w.end());
                }
            }
        }
        if (null) {
            cout << "\n\n";
            accepts(RE, "&");
        }
        else {
            for (int i = 0; i < w.length(); i++) {
                if (!isalpha(w[i]) && !isdigit(w[i]) && !w[i] == '&') {
                    cerr << "Invalid input. Allowed characters [a-z, A-Z, 0-9] \n" << endl;
                    exit(1);
                    //readable = false;
                    //break;
                }
            }
            cout << "\n\n";
            accepts(RE, w);
        }


        //cout << "\n\n";
    } while (readable);

    return 0;
}
