
#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include <array>

#define alphabetsize 2
/************************************************************
 * Description: Checks if input string is valid or not. 
 * Returns: Bool array 
 * *********************************************************/
bool DFA(std::string str, const int tab[][alphabetsize], const bool arr[]) {
	for (int i = 0; i < str.size(); i++) 
	{
		std::cout << str[i] << std::endl; 
	}
	std::cout << std::endl; 

	int index; 
	int state = 0; 

	for (int i = 0; i < str.size(); ++i) 
	{
		index = str[i] - 'a'; 
		std::cout << "index : " << index << " "; 
		state = tab[state][index]; 
		std::cout << "State: " << state << std::endl; 
	}
	return arr[state]; 
}

int main() {
	/*
	*	2D array to hold transition table. 
	*/
	const int table[4][2] = { {1,0}, {1,2}, {1,3}, {1,0} };
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 2; ++j) {
			std::cout << table[i][j] << " "; 
		}
		std::cout << std::endl; 
	}
	//final state table 
	const bool final[4] = { 0,0,0,1 }; 
	std::cout << "Enter string: " << std::endl; 
	std::string input; 
	std::cin >> input; 
	if (DFA(input, table, final))
	{
		std::cout << "valid"; 
	}
	else {
		std::cout << "not valid";
	}
}