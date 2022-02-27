#include "algos.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std; 

enum algorithms{FIFO, LRU, VMS};

// struct pageTableEntry{
//     char rw;
//     int address;
//     bool dirty; 
// };

// extern vector<pageTableEntry> pageTable;

//extern int events = 0, reads = 0, writes = 0; //global variable for data output 
  
int main(int argc, char** argv){
    string fileName, algo, mode;
    int nframes, p, algoNum;

    for (int i = 0; i < 1048576; i++) { //all the page table entries are -1 untill assigned a
        pageTable[i].address = -1;  //a frameNumber within the frame table which the algo decides
    }

    fileName = argv[1];
    nframes = atoi(argv[2]);
    algo = argv[3];
    if(algo == "fifo") algoNum = 0;
    else if(algo == "lru") algoNum = 1;
    if(argv[3] == "vms"){
        algoNum = 2;
        p =  atoi(argv[4]);
        mode = argv[5];  
    }
    else{
        mode = argv[4];
    }
 
    const char *a = fileName.c_str();
    FILE * fp;
    fp = fopen(a, "r");
    if (fp == NULL) {
        cout << "error fp is NULL" << endl;
        return 1;
    }

    unsigned int addr; //32-bit virtual address
    unsigned int pageNum; //found by shifing addr >> 12
    char rw; //read or write
    
    while (fscanf(fp, "%x %c", &addr, &rw) != EOF) {
        events++;
        struct pageTableEntry page;
        page.rw = rw;
        pageNum = addr >> 12; 
        page.address = pageNum; 
        page.dirty = 0;
        pageTable.emplace_back(page);
    }

    switch (algoNum)
    {
    case FIFO:
        fifo(pageTable, nframes);
        break;
    case LRU:
        lru(pageTable, nframes);
        break;
    case VMS:
        vms(pageTable, nframes, p);
        break;
    default:
        cout << "no algoNum present" << endl;
        return -1;
        break;
    }

    cout << "Events: " << events << endl
         << "Table frames: " << nframes << endl
         << "Read count: " << reads << endl
        << "Write count: " << writes << endl;

    fclose (fp);
    return 0;
}























