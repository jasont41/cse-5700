#include <fstream>
#include <iostream>
#include <set>
#include <vector>
#include <cstring>
#include <string>
using namespace std;



class first_follow{
    public: 
        first_follow(string); 
        void add_terminals(); 
        void add_nonterminals();
        void print_vector(vector<char> vect, string str); 
        void print_vector(vector<string>vect, string str); 
        void first_sets(); 
        void follow_sets(); 
        void epsilon_check(); 
        vector<char> get_terminal_vect(){return terminals;}
        vector<char> get_nterminals_vect(){return nonterminals; }
        vector<string> get_working_vect(){return working_vector; }
        
    private: 
        string check_string;
        char first_char;
        vector<char> terminals;
        vector<char> nonterminals;
        vector<string> working_vector;
        set<char> firsts[128];       
        set<char> follows[128];         
        bool hasEpsilon[128]; 
        fstream infile;
}
first_follow::first_follow(string file_name)
{
    infile.open(file_name); 
}
first_follow::add_terminals()
{
    while (first_char != '$')
    {
        terminals.push_back(first_char);
        infile >> first_char;
    }
}
first_follow::add_nonterminals()
{
    infile >> check_string;
    while (check_string != "$")
    {
        first_char = check_string[0];

        nonterminals.push_back(first_char);
        working_vector.push_back(check_string);
        infile >> check_string;
    }
}
first_follow::first_sets()
{
    bool change, isTerminal, derivesEpsilon;
    int oldSize, newSize, checkIndex;
    do
    {
        change = true;
        for (int i = 0; i < working_vector.size(); ++i)
        {
            string toCheck = working_vector[i];
            //cout << "toCheck = " << toCheck << endl;

            char expression = toCheck[0];
            //cout << "expression = " << expression << endl;

            for (int j = 3; j < toCheck.size(); ++j)
            {
                derivesEpsilon = false;
                //cout << "toCheck[j] = " << toCheck[j] << endl;

                //Checks to see if character being checked is a terminal
                isTerminal = false;
                for (int i = 0; i < terminals.size(); ++i)
                {
                    if (terminals[i] == toCheck[j])
                    {
                        isTerminal = true;
                    }
                }
                if (toCheck[j] == 'e' && j == 3)
                {
                    firsts[expression].insert('e');
                    change = true;
                    break;
                    //cout << "Adding e" << endl;
                }
                if (!firsts[toCheck[j]].empty())
                {
                    //cout << "Adding..." << endl;
                    oldSize = firsts[expression].size();
                    for (set<char>::iterator it = firsts[toCheck[j]].begin();
                        it != firsts[toCheck[j]].end(); it++)
                    {
                        if (*it != 'e')
                        {
                            firsts[expression].insert(*it);
                        }
                        else if (*it == 'e')
                        {
                            derivesEpsilon = true;
                        }
                    }
                    newSize = firsts[expression].size();
                    if (oldSize == newSize)
                    {
                        change = false;
                    }
                }
                if (isTerminal || !derivesEpsilon)
                {
                    break;
                }
            }
        }
    } while (change);
}
first_follow::epsilon_check()
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
                    hasEpsilon[i] = true;
                }
            }
        }
    }
}
first_follow::follow_sets()
{
    //Rule one of follows, placing $ in start symbol of G
    string followCheck = working_vector[0];
    //cout << "followCheck = " << followCheck << endl;
    char fChar = followCheck[0];
    //cout << "fChar = " << fChar << endl;
    follows[fChar].insert('$');

    do
    {
        change = true;
        for (int i = 0; i < working_vector.size(); ++i)
        {
            followCheck = working_vector[i];
            fChar = followCheck[0];
            if (followCheck.size() == 6 && followCheck[5] != 'e')
            {
                oldSize = follows[followCheck[4]].size();
                for (set<char>::iterator it = firsts[followCheck[5]].begin();
                    it != firsts[followCheck[5]].end(); ++it)
                {
                    if (*it != 'e')
                    {
                        follows[followCheck[4]].insert(*it);
                    }
                }
                newSize = follows[followCheck[4]].size();
                if (newSize == oldSize)
                {
                    change = false;
                }
            }
            if (followCheck.size() == 6 && hasEpsilon[followCheck[5]])
            {
                oldSize = follows[followCheck[4]].size();
                for (set<char>::iterator it = follows[fChar].begin();
                    it != follows[fChar].end(); ++it)
                {
                    follows[followCheck[4]].insert(*it);
                }
                newSize = follows[followCheck[4]].size();
                if (newSize == oldSize)
                {
                    change = false;
                }
            }
            if (followCheck.size() == 5 && followCheck[4] != 'e')
            {
                oldSize = follows[followCheck[4]].size();
                for (set<char>::iterator it = follows[fChar].begin();
                    it != follows[fChar].end(); ++it)
                {
                    follows[followCheck[4]].insert(*it);
                }
                newSize = follows[followCheck[4]].size();
                if (newSize == oldSize)
                {
                    change = false;
                }
            }
        }
    } while (change);
}
first_follow::print_vector(vector<char> vect, string str)
{
    cout << "Printing  " << str << endl; 
    for(int i = 0; i < vect.size(); i++)
    {
        cout << vect[i] << endl; 
    }
}
first_follow::print_vector(vector<string>vect, string str)
{
    cout << "Printing " << str << endl; 
    for(int i = 0; i < vect.size(); i++){
        cout << vect[i] << endl; 
    }
}

//Displays all the sets.  Takes in the set to be displayed.
void display(set<char> firsts[])
{
    for (int i = 0; i < 128; ++i)
    {
        if (!firsts[i].empty())
        {
            cout << "Firsts(" << (char)i << ") = {";
            for (set<char>::iterator j = firsts[i].begin(); j != firsts[i].end();
                ++j)
            {
                cout << *j << " ";
            }
            cout << "}" << endl;
        }
    }
}

void print_follows(set<char> follows[])
{
    for (int i = 0; i < 128; ++i)
    {
        if (!follows[i].empty())
        {
            cout << "Follows(" << (char)i << ") = {";
            for (set<char>::iterator j = follows[i].begin(); j != follows[i].end();
                ++j)
            {
                cout << *j << " ";
            }
            cout << "}" << endl;
        }
    }
}

int main()
{
    first_follow obj("g419.txt"); 
    obj.add_terminals(); 
    obj.add_nonterminals(); 
    obj.print_vector(obj.get_terminal_vect,"Terminals"); 
    obj.print_vector(obj.get_nterminals_vect, "Non-Terminals"); 
    obj.print_vector(obj.get_working_vect,"Working Vector"); 
    


    /*string check_string;
    char first_char;
    vector<char> terminals;
    vector<char> nonterminals;
    vector<string> working_vector;

    set<char> firsts[128];       
    set<char> follows[128];         

    fstream infile;
    infile.open("g419.txt", ios::in);

    infile >> first_char;

    //Adding terminals to vector
    while (first_char != '$')
    {
        terminals.push_back(first_char);
        infile >> first_char;
    }

    infile >> check_string;

    //Adding nonterminals to vector
    while (check_string != "$")
    {
        first_char = check_string[0];

        nonterminals.push_back(first_char);
        working_vector.push_back(check_string);
        infile >> check_string;
    }

    //To display terminals
    cout << "Terminals: " << endl;
    for (int i = 0; i < terminals.size(); ++i)
    {
        cout << terminals[i] << " ";
    }

    //To display nonterminals
    cout << "Nonterminals: " << endl;
    for (int i = 0; i < nonterminals.size(); ++i)
    {
        cout << nonterminals[i] << " ";
    }
    cout << endl;
    cout << endl;

    //To display working_vector
    cout << "working_vector: " << endl;
    for (int i = 0; i < working_vector.size(); ++i)
    {
        cout << working_vector[i] << endl;
    }
    cout << endl;

    //Rule 1 of First Sets: If X is nonterminal, then First(X) = {X}
    //So, adding terminals to their own index in firsts[]
    for (int i = 0; i < terminals.size(); ++i)
    {
        firsts[terminals[i]].insert(terminals[i]);
    }

    bool change, isTerminal, derivesEpsilon;
    int oldSize, newSize, checkIndex;
    do
    {
        change = true;
        for (int i = 0; i < working_vector.size(); ++i)
        {
            string toCheck = working_vector[i];
            //cout << "toCheck = " << toCheck << endl;

            char expression = toCheck[0];
            //cout << "expression = " << expression << endl;

            for (int j = 3; j < toCheck.size(); ++j)
            {
                derivesEpsilon = false;
                //cout << "toCheck[j] = " << toCheck[j] << endl;

                //Checks to see if character being checked is a terminal
                isTerminal = false;
                for (int i = 0; i < terminals.size(); ++i)
                {
                    if (terminals[i] == toCheck[j])
                    {
                        isTerminal = true;
                    }
                }
                if (toCheck[j] == 'e' && j == 3)
                {
                    firsts[expression].insert('e');
                    change = true;
                    break;
                    //cout << "Adding e" << endl;
                }
                if (!firsts[toCheck[j]].empty())
                {
                    //cout << "Adding..." << endl;
                    oldSize = firsts[expression].size();
                    for (set<char>::iterator it = firsts[toCheck[j]].begin();
                        it != firsts[toCheck[j]].end(); it++)
                    {
                        if (*it != 'e')
                        {
                            firsts[expression].insert(*it);
                        }
                        else if (*it == 'e')
                        {
                            derivesEpsilon = true;
                        }
                    }
                    newSize = firsts[expression].size();
                    if (oldSize == newSize)
                    {
                        change = false;
                    }
                }
                if (isTerminal || !derivesEpsilon)
                {
                    break;
                }
            }
        }
    } while (change);

    display(firsts);
    cout << endl;
    cout << endl;
   
    bool hasEpsilon[128];  
    for (int i = 0; i < 128; ++i)
    {
        if (!firsts[i].empty())
        {
            for (set<char>::iterator it = firsts[i].begin(); it != firsts[i].end();
                ++it)
            {
                if (*it == 'e')
                {
                    hasEpsilon[i] = true;
                }
            }
        }
    }
    
    //Rule one of follows, placing $ in start symbol of G
    string followCheck = working_vector[0];
    //cout << "followCheck = " << followCheck << endl;
    char fChar = followCheck[0];
    //cout << "fChar = " << fChar << endl;
    follows[fChar].insert('$');

    do
    {
        change = true;
        for (int i = 0; i < working_vector.size(); ++i)
        {
            followCheck = working_vector[i];
            fChar = followCheck[0];
            if (followCheck.size() == 6 && followCheck[5] != 'e')
            {
                oldSize = follows[followCheck[4]].size();
                for (set<char>::iterator it = firsts[followCheck[5]].begin();
                    it != firsts[followCheck[5]].end(); ++it)
                {
                    if (*it != 'e')
                    {
                        follows[followCheck[4]].insert(*it);
                    }
                }
                newSize = follows[followCheck[4]].size();
                if (newSize == oldSize)
                {
                    change = false;
                }
            }
            if (followCheck.size() == 6 && hasEpsilon[followCheck[5]])
            {
                oldSize = follows[followCheck[4]].size();
                for (set<char>::iterator it = follows[fChar].begin();
                    it != follows[fChar].end(); ++it)
                {
                    follows[followCheck[4]].insert(*it);
                }
                newSize = follows[followCheck[4]].size();
                if (newSize == oldSize)
                {
                    change = false;
                }
            }
            if (followCheck.size() == 5 && followCheck[4] != 'e')
            {
                oldSize = follows[followCheck[4]].size();
                for (set<char>::iterator it = follows[fChar].begin();
                    it != follows[fChar].end(); ++it)
                {
                    follows[followCheck[4]].insert(*it);
                }
                newSize = follows[followCheck[4]].size();
                if (newSize == oldSize)
                {
                    change = false;
                }
            }
        }
    } while (change);

    print_follows(follows);*/
}