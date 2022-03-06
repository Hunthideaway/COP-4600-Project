/*
Authors: Randall Hunt, Eric Kemmer
Due Date: 03/07/2022
Program: memsim.cpp
*/

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <queue>
#include <deque>
#include <algorithm>
#include <cstdio>
#include <iterator>
#include <cctype>
using namespace std;

void fifo(char *traceFile, unsigned int numFrames, char *exec);             // function for fifo
void lru(char *traceFile, unsigned int numFrames, char *exec);              // function for lru
void vms(char *traceFile, unsigned int numFrames, char *exec, int percent); // function for vms

struct pageData
{ // parameters for the page entries within the trace file
    // variables
    unsigned address;
    char rw;
    bool dirty; // intialize dirty to false
    pageData(unsigned address, char rw)
    { // set the page entry data
        this->address = address;
        this->rw = rw;
    }
    bool operator==(pageData compare)
    { // compares the two pages returning bool for result
        if (compare.address == this->address)
            return true;
        else
            return false;
    }
};

int main(int argc, char *argv[])
{
    char *traceFile;           // trace file that contains pages and data
    char *numFrames;           // number of frames
    char *algo;                // replacement algorithm
    char *exec;                // quiet or debug
    char *vmsArg;              // additional argument for vms
    int percent;               // used to store number for vms additonal argument
    unsigned int numberFrames; // stores the number of frames

    traceFile = strdup(argv[1]);    // second argument from command line should be the trace file
    numFrames = strdup(argv[2]);    // thrid argument from the command line should be number of frames
    numberFrames = atoi(numFrames); // stores the number from string to number
    algo = strdup(argv[3]);         // the algorithm to use within the program
    if (strcmp(algo, "vms") == 0)
    {                             // if the user selects the vms algo then account for the additional input
        vmsArg = strdup(argv[4]); // store the string from the command line
        percent = atoi(vmsArg);   // convert the string to an int
        exec = strdup(argv[5]);   // 6th argument stating either quiet or debug
    }
    else
    {
        exec = strdup(argv[4]); // else the 5th argument if debug or quiet
    }
    char *tolower(algo); // converts to lower case //could not work

    if (strcmp(algo, "fifo") == 0)
    {                                        // if the user inputs fifo
        fifo(traceFile, numberFrames, exec); // call fifo
    }
    else if (strcmp(algo, "lru") == 0)
    {                                       // user inputs lru
        lru(traceFile, numberFrames, exec); // call lru
    }
    else if (strcmp(algo, "vms") == 0)
    {                                                // user inputs vms
        vms(traceFile, numberFrames, exec, percent); // call vms
    }
    else
    {
        // default: inform the user of the correct format needed
        cout << "Invalid number of arguments, format is: ./memsim <tracefile> <frame numbers> <fifo | lru | vms> <quiet | debug> \n if you choose vms include percent before <quiet | debug> " << endl;
    }
    return 0; // end
}

void fifo(char *traceFile, unsigned int numFrames, char *exec) // function for fifo
{
    // variable declarations
    int faults = 0, reads = 0, writes = 0, count = 0;
    deque<pageData> buf;                // algorithm data struct
    FILE *file = fopen(traceFile, "r"); // open file with perms read
    while (!feof(file))
    {                                                                         // continue until you reach end of file
        pageData var(0, ' ');                                                 // intialize the struct parameters
        //deque<pageData>::iterator temp = (find(buf.begin(), buf.end(), var)); // iterator variable
        unsigned int address;
        char rw;
        int verify = fscanf(file, "%x %c", &address, &rw); // scan in data from file
        var.address = address / 4096;                      // calculates page number since each page is of size 4096
        var.rw = rw;                                       // stores current read or write state of the page entry
        if (rw == 'W')
        {                     // is the page entry is a write
            var.dirty = true; // set dirty to true
        }
        else var.dirty = false;  


        if (verify <= 0)
        { // checking if the line was emtpy or null then we move to the next
            continue;
        }

        count++; // increment the count

        if (find(buf.begin(), buf.end(), var) == buf.end())
        { // if we reach the end of the buffer and page is not in page table, page fault
            faults++;
            reads++;
            buf.push_back(var);
            if (strcmp(exec, "debug") == 0)
                cout << "Page fault" << endl; // inform the user of the page fault

            if (buf.size() > numFrames)
            {                                // if there is not space available
                pageData var2 = buf.front(); // go to front of deque
                if (strcmp(exec, "debug") == 0)
                    cout << "The page was evicted!\n"; // if the user selected debug, provide information
                if (var2.dirty == true)
                    writes++; // if the page is dirty increment writes
                if (strcmp(exec, "debug") == 0 && var2.dirty == true)
                    cout << "The page has been written to the disk\n"; // if both cases are true
                buf.pop_front();                                       // pop the front element
            }
            
        }

        else
        {   deque<pageData>::iterator temp = (find(buf.begin(), buf.end(), var));
            if (var.rw == 'W')
                temp->dirty = true; // if rw is write then temp is dirty 
        }
    }
    if (strcmp(exec, "quiet") == 0 || strcmp(exec, "debug") == 0) // if the user selects quiet or debug
    {
        // print out the information to the terminal for the user
        cout << "Frames: " << numFrames << "\n";
        cout << "Count: " << count << "\n";
        cout << "Reads: " << reads << "\n";
        cout << "Writes: " << writes << "\n";
    }
}

void lru(char *traceFile, unsigned int numFrames, char *exec) // function for lru
{
}
void vms(char *traceFile, unsigned int numFrames, char *exec, int percent) // function for vms
{
}
