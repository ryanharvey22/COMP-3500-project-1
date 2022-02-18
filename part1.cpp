// Part 1 : Process Manegment

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <fstream>
#include <ctype.h>
#include <vector>

using namespace std;

/*
*/

struct proc_t{
    int PROCESS_ID;
    int LINE_NUMBER;
    int LETTER_COUNT;
};

vector<proc_t> data;

/*

This system is currently using a shared memory approach.
Each Thread needs to pull from and add to the same set of data so a shared memory system is best.
This method is the most effective in this situation.
The reason why is each process is executed in series so syncronizing accesses is not a problem
(one ofter the other and always finished before the next process starts)

*/

int main(int argc, const char * argv[]){

    ifstream file("samplefile.txt");
    ofstream output("outputfile.txt");
    string line;
    if(!file){
        cout << "Error Opening File\n";
        return -1;
    }

    int max = 0;
    int maxInd = 0;
    int lineNumber = 0;
    
    int total = 0;

    while(getline(file,line)){
        lineNumber++;

        pid_t pid;
        pid = fork();

        if(pid < 0){
            cout << "*** Fork Failed ***\n";
            return 1;
        }
        else if(pid == 0){
            execlp("/bin/ls","ls",NULL);
        }
        else{
            wait(NULL);
            cout << "*** Child Complete ***\n";
        }
        int count = 0;
        for(int i = 0; i < line.length(); i++){
            if(isalpha(line[i])){
                count++;
                total++;
            }
        }
        /* Calculate Max */
        if(count > max){
            max = count;
            maxInd = lineNumber;
        }

        proc_t temp;
        temp.LETTER_COUNT = count;
        temp.LINE_NUMBER = lineNumber;
        temp.PROCESS_ID = pid;

        int pos = -1;
        for(int i = 0; i < data.size(); i++){
            if(data[i].LETTER_COUNT >= temp.LETTER_COUNT){
                pos = i;
                break;
            }
        }
        if(pos == -1){
            data.push_back(temp);
        } else {
            data.insert(data.begin() + pos, temp);
        }
    }

    for(int i = 0; i < data.size(); i++){
        output << "Process ID: " << data[i].PROCESS_ID << endl;
        output << "Line Number: " << data[i].LINE_NUMBER << endl;
        output << "Letter Count: " << data[i].LETTER_COUNT << endl;
        output << "\n";
    } 

    output << "Total Number of Characters: " << total << endl;

    cout << "All Child Proccesses Completed." << endl;
    cout << "Line " << maxInd << " had the maximum number of letters, " << max << "." << endl;
    cout << "See output.txt for full result." << endl;
    cout << "\n";

    return max;

}