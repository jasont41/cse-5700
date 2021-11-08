#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <regex>
using namespace std;

void print_vector(vector<string>);
int check_line(vector<string> &, vector<int> &, string, int);
string get_identifier(string);
bool check_valid_name(string);
string vector_cleaning(string);
int vector_handling(vector<string> &, vector<int> &, string, int);

//global variables
regex regex_str("^[_a-z]\\w*$");

int main()
{
    string line;
    vector<string> identifier;
    vector<int> location;
    ifstream fin("/Users/jasoneastman/Documents/workspace/cse-5700/Lab2/input.txt");

    vector<string> lines;
    if (!fin.is_open())
    {
        cout << "File is no good. Exiting";
        return 0;
    }
    while (getline(fin, line))
    {
        lines.push_back(line);
    }
    for (int i = 0; i < lines.size(); i++)
    {
        //  Return 1 == good, return 8 == ignored main
        int return_code = check_line(identifier, location, lines[i], i);
        //  Uncomment for debugging;
        //  cout << return_code << endl;
    }
    fin.close();
    print_vector(identifier);
}

int check_line(vector<string> &ident, vector<int> &loc, string line, int line_num)
{
    /*
        Note: string.find() returns the index of the beginning of the substring 
    */

    /*
        Vector Cases: 
            vector<data type> var_name (something here); three different, but same catch case 
            vector<data type> var_name {1,2,3,4,5}; array like initialization. name between > and {
            vector<data type> var_name; (nothing between name and semicolon) last case 
    */

    line_num++; //zero based making it one based
    bool found_identifier = false;
    //  Classes 

    if(line.find("class") != string::npos)
    {
        if(line.find("{") == string::npos)
        {
            string substr = line.substr(line.find("class")+5, substr.length()); 
            size_t first = substr.find("{");
            size_t last = substr.find("}", first);
            substr.erase(first, last - first + 1);
            ident.push_back(substr); 
            loc.push_back(line_num); 
            return 1; 
        }
        else
        {
            string substr = line.substr(line.find("class"+5),line.find(";")); 
            ident.push_back(substr); 
            loc.push_back(line_num); 
            return 1; 
        }
    }
    //  struct 
    if(line.find("struct") != string::npos)
    {
        int left_open_bracket_loc = line.find("{"); 
        string substr = line.substr(line.find("struct")+6, line.length()-1); 
        ident.push_back(substr); 
        loc.push_back(line_num); 
        return 1; 
    }

    //function block
    string function_return_types[9] = {"void", "float", "int", "char", "vector", "double", "long", "short", "bool"};
    for (int i = 0; i < 8; i++)
    {
        if (line.find(function_return_types[i]) != string::npos)
        {
            if (function_return_types[i] == "vector") // vector case
            {
                return vector_handling(ident, loc, line, line_num); 
            }
        }
    }

    //variable block
    string data_types[9] = {"char", "short", "long", "int", "float", "double", "bool", "short", "vector"};
    for (int i = 0; i < 8; i++)
    {
        // finds identifier, then isolates declaration
        if (line.find(data_types[i]) != string::npos)
        {
            found_identifier = true;
            int index = line.find(data_types[i]);
            string identif = get_identifier(line.substr(data_types[i].length() + index, line.length()));

            //  Ignore main if there as it wasn't user created and thus isn't a identifier
            if (identif.find("main") != string::npos)
            {
                return 8;
            }
            else if (identif.find("main") == string::npos)
            { //main is NOT in this string
                ident.push_back(get_identifier(line.substr(data_types[i].length() + index, line.length())));
                loc.push_back(line_num);
                return 1;
            }
            //  If you got here, it means the identifer is not a variable.
        }
    }

    return 0;
}

int vector_handling(vector<string> &ident, vector<int> &loc, string line, int line_num)
{
    //case 1
    if (line.find("(") != string::npos)
    {
        int arrow_loc = line.find(">");
        int parenthesis_loc = line.find("(");
        string substr = line.substr(arrow_loc, parenthesis_loc);
        substr = vector_cleaning(substr);

        if (regex_match(substr, regex_str))
        {
            ident.push_back(substr);
            loc.push_back(line_num);
            return 1;
        }
        else
        { //  bad identifier name
            return 8;
        }
    }
    //case 2
    if (line.find("{") != string::npos)
    {
        int arrow_loc = line.find(">");
        int bracket_loc = line.find("{");
        string substr = line.substr(arrow_loc, bracket_loc);
        size_t first = substr.find("{");
        size_t last = substr.find("}", first);
        substr.erase(first, last - first + 1);

        substr = vector_cleaning(substr);

        if (regex_match(substr, regex_str))
        {
            ident.push_back(substr);
            loc.push_back(line_num);
            return 1;
        }
        else
        { //  bad identifier name
            return 8;
        }
    }
    //case 3
    else
    {
        int arrow_loc = line.find(">");
        int semi_colon_loc = line.find(";");
        string substr = line.substr(arrow_loc, semi_colon_loc);
        std::string::iterator end_pos = std::remove(substr.begin(), substr.end(), ' ');
        substr = vector_cleaning(substr);
        substr.erase(end_pos, substr.end());
        ident.push_back(substr);
        loc.push_back(line_num);
        if (regex_match(substr, regex_str))
        {
            ident.push_back(substr);
            loc.push_back(line_num);
            return 1;
        }
        else
        { //  bad identifier name
            return 8;
        }
    }
    return 0; 
}
string vector_cleaning(string line)
{
    std::string::iterator end_pos = std::remove(line.begin(), line.end(), ' ');
    line.erase(end_pos, line.end());
    if (line.find('<') != string::npos)
    {
        std::string::iterator end_pos = std::remove(line.begin(), line.end(), '<');
        line.erase(end_pos, line.end());
    }
    if (line.find('>') != string::npos)
    {
        std::string::iterator end_pos = std::remove(line.begin(), line.end(), '>');
        line.erase(end_pos, line.end());
    }
    if (line.find(';') != string::npos)
    {
        std::string::iterator end_pos = std::remove(line.begin(), line.end(), ';');
        line.erase(end_pos, line.end());
    }
    return line;
}

string get_identifier(string input)
{
    return (input.substr(0, input.find("=")));
}
void print_vector(vector<string> vect)
{
    for (int i = 0; i < vect.size(); i++)
    {
        cout << vect[i] << endl;
    }
}