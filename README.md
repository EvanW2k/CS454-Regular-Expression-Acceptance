# CS454-Final
Prob 6

Implement a program that tests membership of a given string w in a regular expression R.
Specifically, your program will take as input a regular expression (such as (a ∪ a.b)*.(a ∪ E) and a
string w = abaab, and outputs ‘yes’ if w is in L(R), ‘no’ else. 

Three steps are involved in solving this problem: 
1) convert the regular expression to an E-NFA 
2) remove E-moves and 
3) Test if w is accepted by the E-free NFA.
