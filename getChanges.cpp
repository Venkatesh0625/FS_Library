#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

struct change {
    unsigned short int type;
    unsigned long int line_number;
    string line_data;
};

int getLine_(string line)
{
    string nbr;

    for(int i = 4;;i++)
        if(line[i] == ',')
            break;
        else
            nbr.push_back(line[i]);

    return atoi(nbr.c_str());
}

vector<change> updated(string filename)
{
    string str;
    int line_count = 0;
    ifstream file;
    char curr,prev = ' ';
    struct change data;
    vector<change> changes;

    file.open(filename.c_str());

    for(int i=0;i<4;i++)
        getline(file,str);
    while(getline(file, str))
    {   

        line_count++;
        curr = str[0];
        if(curr == '@') {
            line_count = getLine_(str) - 1;
        }
        else if(curr == '+')
            if(prev == '-') { //replace
                data.type = 2;
                data.line_number = --line_count-1;
                data.line_data = str.substr(1);
                changes.push_back(data);
            }
            else{
                data.type = 1;
                data.line_number = --line_count;
                data.line_data = str.substr(1);
                changes.push_back(data);
            } 

        else if(curr == ' ' || curr == '-')
            if(prev == '-') {
                data.type = 3;
                data.line_number = line_count - 1;
                data.line_data = "delete";
                changes.push_back(data);
            }
    

        prev = curr;
    }
    return changes;
}

int main()
{
    vector<change> x = updated("out.txt");
    vector<change>::iterator itr;
    for(itr=x.begin(); itr != x.end(); itr++) {
        cout<<endl<<itr->type<<"\t"<<itr->line_number<<"\t"<<itr->line_data<<endl;
    }
}