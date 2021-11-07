#include <fstream> 
#include <iostream> 
#include <vector> 
#include <string> 
using namespace std; 

void vector_print(vector<string>); 
int check_line(vector<string>&, vector<int>&, string, int); 
int main()
{
    string line; 
    vector<string> identifier; 
    vector<int> location; 
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
    //vector_print(lines); 
    for(int i = 0; i < lines.size(); i++){
        check_line(identifier, location, lines[i], i); 
    }

    
}



int check_line(vector<string>& ident, vector<int>& loc, string line, int line_num)
{
    line_num++;  //zero based making it one based 
    bool found_identifier = false; 
    string sub[7] =  {"char", "short", "long", "int", "float","double","bool"}; 
    for(int i = 0; i < 7; i++)
    {
        if(line.find(sub[i]) != string::npos){
            cout << "found" << endl << i << endl; 
            return 1;  
        }
    }


}

void vector_print(vector<string> vect)
{
    for(int i = 0; i < vect.size(); i++)
    {
        cout << vect[i] << endl; 
    }
}