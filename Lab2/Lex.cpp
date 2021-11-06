#include <fstream> 
#include <iostream> 
#include <vector> 
using namespace std; 

void vector_print(vector<string>); 

int main()
{
    string line; 
    ifstream fin("/Users/jasoneastman/Documents/workspace/cse-5700/Lab2/input.txt"); 

    vector<string> lines; 
    if(!fin.is_open())
    {
        cout << "File is no good. Exiting" ; 
        return 0; 
    }
    while(getline(fin,line))
    {
        lines.push_back(line); 
    }



    
}


void vector_print(vector<string> vect)
{
    for(int i = 0; i < vect.size(); i++)
    {
        cout << vect[i] << endl; 
    }
}