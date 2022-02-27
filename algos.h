#ifndef ALGOS_H
#define ALGOS_H

#include <vector>
using namespace std;


struct pageTableEntry{
    char rw;
    int address;
    bool dirty; 
};

vector<pageTableEntry> pageTable;

int events = 0, reads = 0, writes = 0;


// class algos{
//     public:
//         struct pageTableEntry {char rw; int address; bool dirty; };
//         vector<pageTableEntry> pageTable;
//         void fifo(vector<pageTableEntry> pageTable, int nframes){};
//         void lru(vector<pageTableEntry> pageTable, int nframes){};
//         void vms(vector<pageTableEntry> pageTable, int nframes, int p){};
//     private:
        // struct pageTableEntry{};
        // vector<pageTableEntry> pageTable;

//};
// void fifo(struct pageTableEntry pageTable, int nframes){};
// void lru(struct pageTableEntry pageTable, int nframes){};
// void vms(struct pageTableEntry pageTable, int nframes, int p){};

void fifo(vector<pageTableEntry> pageTable, int nframes){};
void lru(vector<pageTableEntry> pageTable, int nframes){};
void vms(vector<pageTableEntry> pageTable, int nframes, int p){};


#endif