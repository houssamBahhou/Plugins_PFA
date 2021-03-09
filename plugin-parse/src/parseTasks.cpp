#include <iostream>
#include <fstream>
#include <string.h>
#include "parseTasks.hpp"

using namespace std;

void insert_element(std::vector<double> &t, size_t index, double to_insert){
    if(t.size() <= index){
        t.resize(index+1,0);
    }
    t.insert(t.begin()+index,to_insert);
}

int parseTask(string &filename, std::vector<double> &t){
    ifstream input;
    input.open(filename);

    int jobId;
    double startTime;
    double endTime;


    if(!input.is_open())
    {
        return -1;
    }

    while(input) {
        string line;
        getline(input,line,':');

        if(line.compare("JobId") == 0)
        {
            startTime = 0;
            endTime = 0;
            input >> jobId;
        }
        
        if(line.compare("StartTime") == 0 )
        {
            input >> startTime;
        }
        
        if(line.compare("EndTime") == 0 )
        {
            input >> endTime;
            insert_element(t,jobId,endTime - startTime);
        }

        getline(input,line,'\n');
    }

    input.close();
    return 0;
}

double getTime(int jobId, std::vector<double> &t){
    return t[jobId];
}