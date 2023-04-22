#include <iostream>
using namespace std;

/*
    2 inputs
    1) RE as string, with () > U > . > *
    2) String w to test acceptance

    Three steps are involved in solving this problem:

    1. convert the regular expression to an E-NFA
    2. remove E-moves and
    3. Test if w is accepted by the E-free NFA.
*/

int main() {
    string RE = "";
    cout << "Enter regular expression in a single line (U, E acknowledged as union operator, epsilon): " << endl;
    cin >> RE;
    //confirm readable RE
    for (int i = 0; i < RE.length(); i++) {
        if (RE[i] == 'U' || RE[i] == 'E') { //only allowed uppercase symbol
            continue;
        }
        else if (isalpha(RE[i])) {
            RE[i] = tolower(RE[i]);
        }
        /*
        if (RE[i] != '(' || RE[i] != ')' || RE[i] != '.' || RE[i] != '*') {
            cerr << "Invalid input. Allowed characters [a-z, A-Z, (), ., *, U]" << endl;
            exit(1);
        }
        */
    }

    string w = "";
    cout << "Enter string w to be tested against given regular expression: " << endl;
    cin >> w;
    //confirm readable string
    for (int i = 0; i < w.length(); i++) {
        if (isalpha(w[i])) {
            w[i] = tolower(w[i]);
        }
        else {
            cerr << "Invalid input. Allowed characters [a-z, A-Z]." << endl;
            exit(1);
        }
    }
}