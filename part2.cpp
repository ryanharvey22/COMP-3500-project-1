// Part 2 : Thread Manegment

#include <stdio.h>
#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <ctype.h>
#include <sys/types.h>


using namespace std;

struct line_data{
    pthread_t thread_id;
    int letter_count;
    int line_index;
};

vector<line_data> data;

int lineNumber = 0;
int total = 0;

void *get_letter_count(void *_line){
    lineNumber++;
       
    string ln = *static_cast<string*>(_line);

    int count = 0;
    for(int i = 0; i < ln.length(); i++){
        if(isalpha(ln[i])){
            count++;
            total++;
        }
    }

    line_data temp;
    temp.thread_id = pthread_self();
    temp.letter_count = count;
    temp.line_index = lineNumber;

    int pos = -1;
        for(int i = 0; i < data.size(); i++){
            if(data[i].letter_count >= temp.letter_count){
                pos = i;
                break;
            }
        }
        if(pos == -1){
            data.push_back(temp);
        } else {
            data.insert(data.begin() + pos, temp);
        }

    pthread_exit(0);
}

int main(int argc, const char * argv[]){
    int lineNumber = 0;

    ifstream file("samplefile.txt");
    ofstream output("outputfile2.txt");
    string line;
    if(!file){
        cout << "Error Opening File\n";
        return -1;
    }

    while(getline(file,line)){
        lineNumber++;
        int count = 0;

        pthread_t tid;
        pthread_attr_t attr;

        pthread_attr_init(&attr);
        pthread_create(&tid, &attr, &get_letter_count, &line);
        pthread_join(tid, NULL);

    }

    for(int i = 0; i < data.size(); i++){
        output << "Line Number: " << data[i].line_index << endl;
        output << "Letter Count: " << data[i].letter_count << endl;
        output << "\n";
    }

    output << "Total Number of Characters: " << total << endl;

    return 0;
}
    /*
    After assigning each line to a thread and sending each threads data to parent thread,
    calculate max and put data ascending order.
    */
    