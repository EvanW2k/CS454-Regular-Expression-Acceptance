CS454-Final-Problem-6
Created by: Sean Sponsler and Evan Walters
DUE: May 20th, 2023

-----SEE PROJECT REPORT FOR FULL EXPLANATIONS-----

This program takes as input a regular expression, and a string. Then there is an output 
determining if the string is accepted by the regular expression.

The program converts the given RE (regular expression) to an NFA that is then used in conjunction
with the input string to determine if the string is accepted. The program loops for 
continued input. 

The regular expression uses only the operations:
- Concatenation: .
- Union: +
- Kleene Star: *
- Parenthesis: ( and )

The symbols the regular expression uses may be any letter (cap or uncap) or any number.
With the exception that & represents epsilon.


-----Constributions -----------------------------
Sean Sponlser
- Determined our file hiearchy and general program strucure
- implemented main.cpp including input and loop
- main testor/bug fixer
- recursive convert function

Evan Walters
- NFA constructors
- came up with data structure: 2D vector of pairs (resulting state, transition operand)
- bfs (breadth first seach) function
- infix to prefix function for regular expresion

Togethor
- removeEpsilon function for &-NFA -> NFA
- documentation
- wrote up and explained ideas, often with picture of hand done NFAs and &-NFAs
- Project report
- bug fixing




