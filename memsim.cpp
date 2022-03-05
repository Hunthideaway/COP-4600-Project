#include <iostream>
#include <string>
#include <vector>
#include<bits/stdc++.h>
using namespace std; 

int events = 0, reads = 0, writes = 0;

struct pageTableEntry{
    char RW; 
    int address; 
    bool dirty; 
    bool operator == (pageTableEntry eq){
    if(eq.address == this->address)
        return true;
    else    
        return false;
    }   
};


void fifo(vector<pageTableEntry> pageTable, int nframes);

int main(int argc, char** argv){
    enum {FIFO, LRU, VMS};
    string fileName, algo, mode;
    int nframes, p, algoNum;
    vector<pageTableEntry> pageTable;

    // if(argv[1] != NULL){
    //     fileName = argv[1];
    //     nframes = atoi(argv[2]);
    //     algo = argv[3];
    //     if(algo == "fifo") algoNum = 0;
    //     else if(algo == "lru") algoNum = 1;
    //     if(argv[3] == "vms"){
    //         algoNum = 2;
    //         p =  atoi(argv[4]);
    //         mode = argv[5];  
    //     }
    //     else{
    //         mode = argv[4];
    //     }
    // }
   
    fileName = "bzip.trace"; nframes = 64; algo = "fifo"; algoNum = 0; mode = "quiet"; 

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
        page.RW = rw;
        if (rw == 'W') {
            page.dirty = 1;
        }
        pageNum = addr / 4096; 
        page.address = pageNum; 
        pageTable.emplace_back(page);
    }

    switch (algoNum)
    {
    case FIFO:
        fifo(pageTable, nframes);
        break;
    case LRU:
        //lru(pageTable, nframes);
        break;
    case VMS:
        //vms(pageTable, nframes, p);
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




void fifo(vector<pageTableEntry> pageTable, int nframes){
	deque<pageTableEntry> deq;
	for (int i=0; i<pageTable.size(); i++){ 
		if (find(deq.begin(), deq.end(), pageTable[i]) == deq.end()) { //if the page is not in page table
            reads++;
            deq.push_back(pageTable[i]);
            if (deq.size() > nframes) {
                pageTableEntry temp = deq.front();
                deq.pop_front();
            }
        }
        else { //if page is in the page table
            deque<pageTableEntry>::iterator it = (find(deq.begin(), deq.end(), pageTable[i]));
            if (pageTable[i].RW == 'W') {
                (*it).dirty = 1;
            }
        }

    }
   
}





















void fifo(vector<pageTableEntry> pageTable, int nframes){
	deque<pageTableEntry> deq;
    for(int i = 0; i < pageTable.size(); i++){
        if(find(deq.begin(), deq.end(), pageTable[i]) == deq.end()){ // if page is not in the table
            reads++;                                                
            deq.push_back(pageTable[i]);                             
            if(deq.size() > nframes){                                //if deq is larger than nframes
                pageTableEntry temp = deq.front();                    
                if(temp.dirty == 1){
                    writes++;
                }
                deq.pop_front(); 
            } 
        }
        else{ //page is in the page table
        deque<pageTableEntry>:: iterator it  = (find(deq.begin(), deq.end(), pageTable[i]));
            if(pageTable[i].RW == 'W'){
                (*it).dirty = 1; 
            }
        }
    }
}













	for (int i=0; i<pageTable.size(); i++){ 
		if (frameTable.size() < nframes){ //if the frameTable is not full
			if (frameTable.count(pageTable[i].address) == 0){ //if the desired page is not in the frame table
				if(pageTable[i].RW == 'R'){//read not in the frame table .... count as read and load in page table
                    reads++; //incrementing global variable reads
					frameTable.insert(pageTable[i].address); //inserting into frameTable
					q.push(pageTable[i].address);
				}
				else if(pageTable[i].RW == 'W'){//write not in the frame table ... count as ready , mark as dirty , load in the page table
					reads++; //incrementing global variable reads
					pageTable[i].dirty = 1; //marking as dirty
					frameTable.insert(pageTable[i].address); //inserting into frameTable
					q.push(pageTable[i].address); //pushing to q
				}
			}
            else if(frameTable.count(pageTable[i].address) == 1){//frameTable is not full and desired page is in the frame table
                if(pageTable[i].RW == 'W'){
                    pageTable[i].dirty = 1;
                }
            } 
		}
		else{ //if the frame table is full
			if (frameTable.count(pageTable[i].address) == 0){ //the desired page is not in the full frame table
				reads++; //if the page is not in frame table then we are adding it and regaurdless of it is a W or R we still count a read
				int val = q.front(); //val is = to the front of the queue

				//val is the address that needs to be evicted
				//	if it was dirty then count it as a write 
                //going through the pageTable 
                //  if the matching address to val is also dirty count as write
				for(int k = 0; k < i; k++){
                    if(pageTable[k].address == val && pageTable[k].dirty == 1){
                         writes++;
                         k = 1000000;
                    }

                }
				q.pop(); //removing the page at the front of the queue
				frameTable.erase(val); //removing page that was located at the front of the q from the page table
				frameTable.insert(pageTable[i].address); //inserting the new desired page into the newest spot in the frameTable
				q.push(pageTable[i].address); //inserting the new desired page into the back of the queue
			}
            else{//frameTable is full and desired page is in the frame table
                if(pageTable[i].RW == 'W'){
                    pageTable[i].dirty = 1;
                }
            } 
		}
	}












void fifo(vector<pageTableEntry> pageTable, int nframes){
	unordered_set<int> frameTable;
	queue<int> q;
	for (int i=0; i<1000000; i++){ 
		if (frameTable.size() < nframes){ //if the frameTable is not fulll
			if (frameTable.find(pageTable[i].address)==frameTable.end()){ //if the desired page is not in the frame table
				if(pageTable[i].rw == 'R'){//read not in the frame table .... count as read and load in page table
                    reads++; //incrementing global variable reads
					frameTable.insert(pageTable[i].address); //inserting into frameTable
					q.push(pageTable[i].address);
				}
				else if(pageTable[i].rw == 'W'){//write not in the frame table ... count as ready , mark as dirty , load in the page table
					reads++; //incrementing global variable reads
					pageTable[i].dirty = 1; //marking as dirty
					frameTable.insert(pageTable[i].address); //inserting into frameTable
					q.push(pageTable[i].address); //pushing to q
				}
			}
            else{//frameTable is not full and desired page is in the frame table
                if(pageTable[i].rw == 'W'){
                    pageTable[i].dirty = 1;
                }
            } 
		}
		else{ //if the frame table is full
			if (frameTable.find(pageTable[i].address) == frameTable.end()){ //the desired page is not in the full frame table
				reads++; //if the page is not in frame table then we are adding it and regaurdless of it is a W or R we still count a read
				int val = q.front(); //val is = to the front of the queue

				//val is the address that needs to be evicted
				//	if it was dirty then count it as a write 
                //going through the pageTable 
                //  if the matching address to val is also dirty count as write
				for(int k = 0; k < i; k++){
                    if(pageTable[k].address == val && pageTable[k].dirty == 1){
                         writes++;
                         k = 1000000;
                    }

                }
				q.pop(); //removing the page at the front of the queue
				frameTable.erase(val); //removing page that was located at the front of the q from the page table
				frameTable.insert(pageTable[i].address); //inserting the new desired page into the newest spot in the frameTable
				q.push(pageTable[i].address); //inserting the new desired page into the back of the queue
			}
            else{//frameTable is full and desired page is in the frame table
                if(pageTable[i].rw == 'W'){
                    pageTable[i].dirty = 1;
                }
            } 
		}
	}
}