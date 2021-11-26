/*
CSE 5700 
Winter 2021 
Eastman, Jason

Requirements:
    1. Reads a grammar.
    2. Finds all FIRST(X) sets, following algorithm on pg. 189 of text.
    3. Finds all FOLLOW(A) sets following algorithm on pg. 190 of text.
Inputs: 
    A Grammer G 
Outputs: 
    The list of all productions in G
	The FIRST(X) and FOLLOW(A) sets of G
*/


#include <fstream>
#include <iostream>
#include <set>
#include <vector>
#include <cstring>

int main()
{
    std::string check_string; 
    char first_check; 
    std::fstream infile; 
    std::set<char> firsts[128]; 
    std::set<char> follow[128]; 
    std::vector<char> terminal; 
    std::vector<char> nonterminal; 
    infile.open("g419.txt", std::ios::in); 

    infile >> first_check; 

    while(first_check != '$')
    {

    }
}