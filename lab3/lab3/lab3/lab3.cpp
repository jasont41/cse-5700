#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <set>


using namespace std; 

bool firstProduction = true;
void printAllGrammar();
bool isTerminal(char);
void first();
void follow();
void printGrammar();

string file = "g.txt";
vector<char> terminal;
vector<char> nonTerminal;
map<char, string> production;
map<char, set<char> > firstMap;
map<char, set<char> > followMap;

int main()
{
    string line; 
    fstream in; 
    in.open(file.c_str(), ios::in);
    if (!in)
    {
        cout << "Error opening in file"; 
        return 0; 
    }
    getline(in, line); 
    while (!in.eof())
    {
        char value = line[0]; 
        if (islower(value) || value == '+' || value == '*' || value == '(' || value == ')')
        {
            terminal.push_back(value);
        }
        if (isupper(value))
        {
            if (nonTerminal.empty())
                nonTerminal.push_back(value);
            else if (!(find(nonTerminal.begin(), nonTerminal.end(), value) != nonTerminal.end()))
                nonTerminal.push_back(value);

            if (production[value] == "")
                production[value] = line.substr(3, line.length());
            else
                production[value] += "|" + line.substr(3, line.length());
        }
        getline(in, line); 
    }
    in.close();
}

/************************************************************
 * Description: Prints grammer
 * Returns: Nothing 
 * *********************************************************/
void printAllGrammar()
{
    for (int i = 0; i < nonTerminal.size(); i++)
    {
        cout << "First(" << nonTerminal[i] << ") = {";
        for (auto it = firstMap[nonTerminal[i]].begin(); it != firstMap[nonTerminal[i]].end(); it++)
        {
            cout << *it;
            auto it2 = next(it, 1);
            if (it2 != firstMap[nonTerminal[i]].end())
            {
                cout << ",";
            }
        }

        cout << "}" << endl;
    }
    for (int i = 0; i < nonTerminal.size(); i++)
    {
        cout << "Follow(" << nonTerminal[i] << ") = {";
        for (auto it = followMap[nonTerminal[i]].begin(); it != followMap[nonTerminal[i]].end(); it++)
        {
            cout << *it;
            auto it2 = next(it, 1);
            if (it2 != followMap[nonTerminal[i]].end())
            {
                cout << ",";
            }
        }
        cout << "}" << endl;
    }
}
/************************************************************
 * Description: Checks if char is a terminal or not 
 * Returns: Bool
 * *********************************************************/
bool isTerminal(char in)
{
    if (in == 'e')
        return true;
    if (find(terminal.begin(), terminal.end(), in) != terminal.end())
        return true;
    else
        return false;
}
/************************************************************
 * Description: Checks if first set or not
 * Returns: Nothing 
 * *********************************************************/
void first()
{
    char left_hand;
    bool done = false;
    bool changes = false;
    int i = 0;

    while (!done)
    {
        left_hand = nonTerminal[i++ % nonTerminal.size()];
        i = i % nonTerminal.size();
        if (i == 0)
            changes = false;

        int j = 0;
        bool firstChar = true;
        string expression = production[left_hand];

        while (j < expression.length())
        {
            char first;
            if (firstChar == true)
            {
                firstChar = false;
                first = expression[0];
            }
            else
            {
                char temp = expression[j];
                while (temp != '|' && j <= expression.length())
                    temp = expression[j++];
                if (j >= expression.length())
                    continue;
                first = expression[j];
            }
            if (isTerminal(first))
            {
                set<char> builder = firstMap[left_hand];
                bool found = builder.find(first) != builder.end();
                if (!found)
                {
                    firstMap[left_hand].insert(first);
                    changes = true;
                }
            }
            else
            {
                set<char> next = firstMap[first];
                for (auto k = next.begin(); k != next.end(); k++)
                {
                    char temp = *k;
                    bool found = firstMap[left_hand].find(temp) != firstMap[left_hand].end();
                    if (!found)
                    {
                        set<char> builder = firstMap[left_hand];
                        firstMap[left_hand].insert(temp);
                        changes = true;
                    }
                }
            }
        }

        if (i == nonTerminal.size() - 1 && changes == false)
            done = true;
    }
}
/************************************************************
 * Description: checks if follow set or not 
 * Returns: Nothing
 * *********************************************************/
void follow() 
{
    char left_hand;
    bool complete = false;
    bool changes = false;
    int i = 0;

    while (!complete)
    {
        left_hand = nonTerminal[i++ % nonTerminal.size()];
        i = i % nonTerminal.size();
        if (i == 0)
            changes = false;

        int j = 0;
        bool first = true;
        string expression = production[left_hand];

        while (j < expression.length())
        {
            char firstChar;
            char followChar;

            if (first == true)
            {
                firstChar = expression[0];
                followChar = expression[1];
                if (isTerminal(firstChar))
                {
                    j++;
                    firstChar = expression[j];
                    followChar = expression[j + 1];
                }
                first = false;
                j++;
            }
            else
            {
                firstChar = expression[j];
                followChar = expression[j + 1];
                while (firstChar == '|' || followChar == '|' && !isTerminal(firstChar))
                {
                    firstChar = expression[j];
                    followChar = expression[j + 1];
                    j++;
                }

                j++;
            }

            if (isTerminal(firstChar) == false and followChar == '\0')
            {
                set<char> builder = followMap[left_hand];
                set<char> builder2 = followMap[firstChar];
                for (auto k = builder.begin(); k != builder.end(); k++)
                {
                    bool found = followMap[firstChar].find(*k) != followMap[firstChar].end();
                    if (!found && *k != 'e')
                    {
                        followMap[firstChar].insert(*k);
                        changes = true;
                    }
                }
            }

            if (isTerminal(firstChar) == false and isTerminal(followChar) == true)
            {
                if (followChar != 'e')
                {
                    bool found = followMap[firstChar].find(followChar) != followMap[firstChar].end();
                    if (!found)
                    {
                        followMap[firstChar].insert(followChar);
                        changes = true;
                    }
                }
            }

            if (isTerminal(firstChar) == false and isTerminal(followChar) == false)
            {
                set<char> builder = followMap[firstChar];
                set<char> builder2 = firstMap[followChar];
                set<char> builder3 = followMap[left_hand];
                bool done = false;
                for (auto k = builder2.begin(); k != builder2.end(); k++)
                {
                    if (*k == 'e')
                        done = true;
                    if (*k != 'e')
                    {
                        bool found = followMap[firstChar].find(*k) != followMap[firstChar].end();
                        if (!found)
                        {
                            followMap[firstChar].insert(*k);
                            changes = true;
                        }
                    }
                }

                if (done)
                {
                    for (auto k = builder3.begin(); k != builder3.end(); k++)
                    {
                        if (*k != 'e')
                        {
                            bool found = followMap[firstChar].find(*k) != followMap[firstChar].end();
                            if (!found)
                            {
                                followMap[firstChar].insert(*k);
                                changes = true;
                            }
                        }
                    }
                }
            }

            if (i == nonTerminal.size() - 1 && changes == false)
                complete = true;
        }
    }
    followMap[nonTerminal[0]].insert('$');
}
/************************************************************
 * Description: Prints Grammar
 * Returns: Nothing 
 * *********************************************************/
void printGrammar()
{
    cout << "Grammar" << endl;
    for (int i = 0; i < terminal.size(); i++)
        cout << terminal[i] << endl;

    cout << "$" << endl;

    for (int i = 0; i < nonTerminal.size(); i++)
        cout << nonTerminal[i] << "->" << production[nonTerminal[i]] << endl;

    cout << "$" << endl;
}
