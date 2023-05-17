# CS454-Final
Created by: Sean Sponsler and Evan Walters
Problem 6
DUE: May 20th, 2023

This program takes as input a regular expression, and a string. Then there is an output 
determining if the string is accepted by the regualr expression.

The program converts the given RE (regualr expression) to an NFA that is then used in conjunction
with the input string to determine if the string is accepted. The program loops for 
continued input. 

The regular expression uses only the operations:
- Concatenation: .
- Union: +
- Kleen Star: *
- Parenthesis: ( and )

The symbols the regular expression uses may be any single characters other than the above operators.
With the exception that & represents epsilon.

Algorithms Used in program:
- InFix to Prefix of RE
- Prefix RE to &-NFA
- &-NFA to NFA
- Breadth first search with input string and NFA

Sample Inputs and Outputs:

#1




