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
void terminal_storage(char a, std::fstream fin, std::vector<char> &store)
{
    fin >> a;
    while (a != '$')
    {
        store.push_back(a);
        fin >> a;
    }
}
void non_terminal_storage(std::string a, std::fstream fin, std::vector<std::string> &work_vec, std::vector<char> &vect)
{
    fin >> a;
    while (a != "$")
    {
        char temp = a[0];
        vect.push_back(temp);
        work_vec.push_back(a);
        fin >> a;
    }
}
int main()
{
    std::string check_string;
    std::string check_follow; 
    char first_check;
    std::fstream infile;
    std::set<char> firsts[128];
    std::set<char> follow[128];
    std::vector<char> terminal;
    std::vector<char> nonterminal;
    std::vector<std::string> work_vec;
    infile.open("g419.txt");

    terminal_storage(first_check, infile, terminal);
    non_terminal_storage(check_string, infile, work_vec, nonterminal);

    for (int index = 0; index < terminal.size(); index++)
    {
        firsts[terminal[index]].insert(terminal[index]);
    }
    bool change, is_terminal, dEpsilon;
    int olds, news, checkI;

    do
    {
        change = true;
        for (int index = 0; index < work_vec.size(); index++)
        {
            check_string = work_vec[index];
            char temp = check_string[0];
            for (int j = 3; j < check_string.size(); ++j)
            {
                dEpsilon = false;
                is_terminal = false;
                for (int count = 0; count < terminal.size(); count++)
                {
                    if (terminal[count] == check_string[j])
                    {
                        is_terminal = true;
                    }
                }
                if (check_string[j] == 'e' && j == 3)
                {
                    firsts[temp].insert('e');
                    change = true;
                    break;
                }
                if (!firsts[check_string[j]].empty())
                {
                    olds = firsts[temp].size();
                    for (std::set<char>::iterator it = firsts[check_string[j]].begin(); it != firsts[check_string[j]].end(); it++)
                    {
                        if (*it != 'e')
                        {
                            firsts[temp].insert(*it);
                        }
                        else if (*it == 'e')
                        {
                            dEpsilon = true;
                        }
                    }
                    news = firsts[temp].size();
                    if (olds == news)
                    {
                        change = false;
                    }
                }
                if (is_terminal || !dEpsilon)
                {
                    break;
                }
            }
        }
    } while (change);

    for (int i = work_vec.size(); i >= 0; --i)
    {
        check_string.clear();
        check_string = work_vec[i];
        char temp = check_string[0];
        for (int j = 3; j < check_string.size(); ++j)
        {
            dEpsilon = false;
            is_terminal = false;
            for (int i = 0; i < terminal.size(); i++)
            {
                if (terminal[i] == check_string[j])
                {
                    is_terminal = true;
                }
            }
            if (check_string[j] == 'e' && j == 3)
            {
                firsts[temp].insert('e');
                change = true;
                break;
            }
            if (!firsts[check_string[j]].empty())
            {
                olds = firsts[temp].size();
                for (std::set<char>::iterator it = firsts[check_string[j]].begin(); it != firsts[check_string[j]].end(); it++)
                {
                    if (*it != 'e')
                    {
                        firsts[temp].insert(*it);
                    }
                    else if (*it == 'e')
                    {
                        dEpsilon = true;
                    }
                }
                news = firsts[temp].size();
                if (olds == news)
                {
                    change = false;
                }
            }
            if(is_terminal || !dEpsilon){
                break; 
            }
        }
    }
    bool hEpsilon[128]; 
    for (int i = 0; i < 128; i++)
    {
        if(!firsts[i].empty())
        {
            for (std::set<char>::iterator it = firsts[i].begin(); it != firsts[i].end();++it)
            {
                if(*it == 'e')
                {
                    hEpsilon[i] == true; 
                }
            }
        }
    }
    check_follow = work_vec[0]; 
    char followch = check_follow[0]; 
    follow[followch].insert('$'); 
    do
    {
        change = true; 
        followch = check_follow[0]; 
        if(check_follow.size() == 6 && check_follow[5] != 'e')
        {
            olds = follow[check_follow[4]].size(); 
            for(std::set<char>::iterator it = firsts[check_follow[5]].begin(); it != firsts[check_follow[5]].end(); ++it)
            {
                if(*it != 'e')
                {
                    follow[check_follow[4]].insert(*it); 
                }

            }
            news = follow[check_follow[4]].size(); 
            if(news == olds) 
            {
                change = false; 
            }
        }
        if(check_follow.size() == 6 && hEpsilon[check_follow[5]])
        {
            olds = follow[check_follow[4]].size(); 
            for(std::set<char>::iterator it = follow[followch].begin(); it != follow[followch].end(); ++it)
            {
                follow[check_follow[4]].insert(*it); 
            }
            news = follow[check_follow[4]].size(); 
            if(news == olds){
                change = false; 
            }
        }
        if(check_follow.size() == 5 && check_follow[4] != 'e')
        {
            olds = follow[check_follow[4]].size(); 
            for(std::set<char>::iterator it = follow[followch].begin(); it != follow[followch].end(); ++it)
            {
                follow[check_follow[4]].insert(*it); 
            }
            news = follow[check_follow[4]].size(); 
        }
        if(news == olds)
        {
            change = true; 
        }
    }while(change); 
}