#include <fstream>
#include <iostream>
#include <set>
#include <vector>
#include <cstring>

using namespace std;

/************************************************************
 * Description: Prints incoming set using iterators 
 * Returns    : Nothing
 * *********************************************************/
void print_set(set<char>a[]) {
    for (int i = 0; i < 128; ++i)
    {
        if (!a[i].empty())
        {
            cout << "(" << (char)i << ") = {";
            for (set<char>::iterator j = a[i].begin(); j != a[i].end();
                ++j)
            {
                cout << *j << " ";
            }
            cout << "}" << endl;
        }
    }
}
/************************************************************
 * Description: Prints incoming vector, to clean up main()
 * Returns    : Nothing
 * *********************************************************/
void print_vect(vector<char> a) {
    for (int i = 0; i < int(a.size()); i++)
    {
        cout << a[i] << endl;
    }
}
/************************************************************
 * Description: Prints incoming vector, overloaded definition
 * Returns    : Nothing
 * *********************************************************/
void print_vect(vector<string> a) {
    for (int i = 0; i < int(a.size()); i++)
    {
        cout << a[i] << endl;
    }
}
/************************************************************
 * Description: checks for epsilon in first set, clean up main()
 * Returns    : Nothing
 * *********************************************************/
void has_epsilon(bool hEP[], set<char> firsts [])
{
    for (int i = 0; i < 128; ++i)
    {
        if (!firsts[i].empty())
        {
            for (set<char>::iterator it = firsts[i].begin(); it != firsts[i].end();
                ++it)
            {
                if (*it == 'e')
                {
                    hEP[i] = true;
                }
            }
        }
    }
}
int main()
{
    string check_string;
    char check_char;
    vector<char> terminals;
    vector<char> nonterminals;
    vector<string> productions;
    string filename = "g417.txt"; 
    set<char> firsts[128];
    set<char> follows[128];

    fstream input;
    cout << "Grammar  " << filename.substr(0, filename.length() - 4) << endl ;
    input.open("g419.txt", ios::in);

    input >> check_char;


    //  Check for terminals 
    while (check_char != '$')
    {
        terminals.push_back(check_char);
        input >> check_char;
    }

    input >> check_string;

    //  Check for nonterminals and productions 
    while (check_string != "$")
    {
        check_char = check_string[0];

        nonterminals.push_back(check_char);
        productions.push_back(check_string);
        input >> check_string;
    }
    //  print all three vectors 
    cout << "Terminals: " << endl;
    print_vect(terminals);

    cout << "Nonterminals: " << endl;
    print_vect(nonterminals);

    cout << "Productions: " << endl;
    print_vect(productions);

    for (int i = 0; i < int(terminals.size()); ++i) //  int case to clear signed/unsigned mismatch error 
    {
        //  Inserting terminals into first set Set 
        firsts[terminals[i]].insert(terminals[i]);
    }

    bool change = true, isTerminal, dEpsilon;
    int olds, news; // old and new sizes 
    do
    {
        change = true;
        for (int i = 0; i < int(productions.size()); ++i)
        {
            string toCheck = productions[i];
            char expression = toCheck[0];

            for (int j = 3; j < int(toCheck.size()); ++j)
            {
                dEpsilon = false;
                isTerminal = false;
                for (int i = 0; i < int(terminals.size()); ++i)
                {
                    // check if check string is an existing terminal 
                    if (terminals[i] == toCheck[j])
                    {
                        isTerminal = true;
                    }
                }
                // check if e is in the third position 
                if (toCheck[j] == 'e' && j == 3)
                {
                    firsts[expression].insert('e');
                    change = true;
                    break; //   drop out of loop 
                }
                //see if that position of set is empty 
                if (!firsts[toCheck[j]].empty())
                {
                    olds = firsts[expression].size();
                    for (set<char>::iterator it = firsts[toCheck[j]].begin(); it != firsts[toCheck[j]].end(); it++)
                    {   
                        //  if dereferenced iterator isn't e, insert into first set 
                        if (*it != 'e')
                        {
                            firsts[expression].insert(*it);
                        }
                        else if (*it == 'e')
                        {
                            dEpsilon = true;
                        }
                    }
                    news = firsts[expression].size();
                    if (olds == news)
                    {
                        change = false;
                    }
                }
                if (isTerminal || !dEpsilon)
                {
                    break;
                }
            }
        }
    } while (change);
    print_set(firsts);
    
    // array to store if each set item has an epilson 
    bool hasEpsilon[128] = {};
    //  moved to clean up main 
    has_epsilon(hasEpsilon, firsts); 
   
    // check productions to see if they are follow sets 
    string followCheck = productions[0];
    char fChar = followCheck[0];
    follows[fChar].insert('$');
    do
    {
        change = true;
        for (int i = 0; i < int(productions.size()); ++i)
        {
            followCheck = productions[i];
            fChar = followCheck[0];
            //  see if epsilon is in the 6th spot and the string is 7 chars long 
            if (followCheck.size() == 6 && followCheck[5] != 'e')
            {
                olds = follows[followCheck[4]].size();
                for (set<char>::iterator it = firsts[followCheck[5]].begin();
                    it != firsts[followCheck[5]].end(); ++it)
                {
                    // same as first sets 
                    if (*it != 'e')
                    {
                        follows[followCheck[4]].insert(*it);
                    }
                }
                news = follows[followCheck[4]].size();
                if (news == olds)
                {
                    change = false;
                }
            }
            //must have epsilon to be a follow 
            if (followCheck.size() == 6 && hasEpsilon[followCheck[5]])
            {
                olds = follows[followCheck[4]].size();
                for (set<char>::iterator it = follows[fChar].begin();
                    it != follows[fChar].end(); ++it)
                {
                    follows[followCheck[4]].insert(*it);
                }
                news = follows[followCheck[4]].size();
                if (news == olds)
                {
                    change = false;
                }
            }
            if (followCheck.size() == 5 && followCheck[4] != 'e')
            {
                olds = follows[followCheck[4]].size();
                for (set<char>::iterator it = follows[fChar].begin();
                    it != follows[fChar].end(); ++it)
                {
                    follows[followCheck[4]].insert(*it);
                }
                news = follows[followCheck[4]].size();
                if (news == olds)
                {
                    change = false;
                }
            }
        }
    } while (change);
    print_set(follows);
}