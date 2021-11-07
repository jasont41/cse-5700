#include <fstream> 
#include <iostream> 
#include <vector> 
#include <string> 
using namespace std; 

void print_vector(vector<string>); 
int check_line(vector<string>&, vector<int>&, string, int); 
string get_identifier(string); 
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
        //  Return 1 == good, return 8 == ignored main
        int return_code = check_line(identifier, location, lines[i], i); 
        //  Uncomment for debugging; 
        //cout << return_code << endl; 
    }
    
    print_vector(identifier); 
}



int check_line(vector<string>& ident, vector<int>& loc, string line, int line_num)
{
    line_num++;  //zero based making it one based 
    bool found_identifier = false; 
    string sub[7] =  {"char", "short", "long", "int", "float","double","bool"}; 
    for(int i = 0; i < 7; i++)
    {
        // finds identifier, then isolates declaration  
        if(line.find(sub[i]) != string::npos){  

            int index = line.find(sub[i]); 
            string identif = get_identifier(line.substr(sub[i].length()+index, line.length())); 

            //  Ignore main if there as it wasn't user created and thus isn't a identifier 
            if(identif.find("main") != string::npos)
            {
                return 8;  
            }
            else
            {
                ident.push_back(get_identifier(line.substr(sub[i].length()+index, line.length())));
                loc.push_back(line_num); 
                return 1; 
            }
        }
    }
    return 0; 
}



string get_identifier(string input)
{
    return (input.substr(0,input.find("="))); 
}
void print_vector(vector<string> vect)
{
    for(int i = 0; i < vect.size(); i++)
    {
        cout << vect[i] << endl; 
    }
}