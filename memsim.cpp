#include <iostream>

#include <fstream>

#include <vector>

#include <cstring>

#include <string>

#include <queue>

#include <deque>

#include <algorithm>

#include <cstdio>

#include <iterator>

#include <set>

#include<bits/stdc++.h> 

using namespace std; 

int events = 0, reads = 0, writes = 0;

struct pageTableEntry{
    char rw; 
    int address; 
    bool operator == (pageTableEntry cmp) {
        if (cmp.address == this->address)
            return true;
        else
            return false;
    }
};

void fifo(vector<pageTableEntry> pageTable, int nframes, string mode);
void lru(vector<pageTableEntry> pageTable, int nframes, string mode);


int main(int argc, char** argv){
    enum {FIFO, LRU, VMS};
    string fileName, algo, mode;
    int nframes, p, algoNum;
    vector<pageTableEntry> pageTable;

    if(argv[1] != NULL){
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
        pageNum = addr >> 12; // / 4096 
        page.address = pageNum; 
        pageTable.push_back(page);
    }

    switch (algoNum)
    {
    case FIFO:
        fifo(pageTable, nframes, mode);
        break;
    case LRU:
        lru(pageTable, nframes, mode);
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

void printDeq(deque<pageTableEntry> d){
    cout << "deq: " << endl;
    cout << "*************************************" << endl;
    for (int i=0; i<d.size(); i++) {
        cout << d[i].address << " " << d[i].rw << endl;
    }
    cout << "*************************************" << endl;
}

bool pageFound(deque<pageTableEntry> d, pageTableEntry p){
    for(int i = 0; i < d.size(); i++){
        if(d[i].address == p.address){
            return true;
        }
    }
    return false;
}

void updateWrite(deque <pageTableEntry>& d, pageTableEntry p){
    for(int k = 0; k < d.size(); k++){
        if(d[k].address == p.address){
            d[k].rw = 'W';
        }
    }
}

void fifo(vector<pageTableEntry> pageTable, int nframes, string mode){
	deque <pageTableEntry> deq;
    cout << "fifo: " << endl;
    for(int i = 0; i < pageTable.size(); i++){
        if(mode == "debug") printDeq(deq);
        if(pageFound(deq, pageTable[i]) == true){   //page is in fifo 
            if(mode == "debug") cout << "page found: " << pageTable[i].address << " " << pageTable[i].rw << endl;
            if(pageTable[i].rw == 'W'){
                if(mode == "debug") cout << "Updating rw of: " << pageTable[i].address << endl;
                updateWrite(deq, pageTable[i]);
            }
        }
        else if(pageFound(deq, pageTable[i]) == false){ //page is not in fifo
            if(mode == "debug") cout << "page not found in fifo: " << pageTable[i].address << " " << pageTable[i].rw << endl;
            if(deq.size() < nframes){      //fifo not full
                if(mode == "debug") cout << "deq size: " << deq.size() << " fifo not full" << endl;
                reads++;
                if(mode == "debug") cout << "incrementing reads ... read = " << reads << endl;
                if(mode == "debug") cout << "pushing " << pageTable[i].address << " " << pageTable[i].rw << endl;
                deq.push_back(pageTable[i]);
            }    
            else if(deq.size() == nframes){                          //fifo full
                if(mode == "debug") cout << "deq size: " << deq.size() << " fifo full" << endl;
                reads++;
                if(mode == "debug") cout << "incrementing reads ... read = " << reads << endl;
                pageTableEntry victim = deq.front();
                if(victim.rw == 'W'){
                    if(mode == "debug") cout << victim.address << " " << victim.rw << " was a write "<< endl;
                    writes++;
                    if(mode == "debug") cout << "incremted writes to " << writes << endl;
                }
                if(mode == "debug") cout << "Evicting entry: " << victim.address << " " << victim.rw << endl;
                deq.pop_front();
                if(mode == "debug") cout << "pushing entry: " << pageTable[i].address << " " << pageTable[i].rw << endl;
                deq.push_back(pageTable[i]);
            }
        }
    }
}

void move_front(deque <pageTableEntry>& d, pageTableEntry p){
    int index;
    bool perserveWrite = false; //perserve write is some voodoo 

    for(int i = 0; i < d.size(); i++){
        if(d[i].address == p.address){
            if(d[i].rw == 'W'){
                perserveWrite = true;
            }
            index = i;
            i = d.size();
        }
    }
    deque<pageTableEntry>::iterator it;
    it = d.begin();
    for(int j = 0; j < index; j++){
        it++;
    }
    d.erase(it);
    if(perserveWrite){
        pageTableEntry ne; //new entry to perserve write 
        ne.address = p.address;
        ne.rw = 'W';
        d.push_front(ne);
    }
    else{
        d.push_front(p);
    }
}


void lru(vector<pageTableEntry> pageTable, int nframes, string mode){
    cout << "LRU: " << endl;
    deque <pageTableEntry> deq;
    for(int i = 0; i < pageTable.size(); i++){
        if(mode == "debug") printDeq(deq);
        if(pageFound(deq, pageTable[i]) == true){   //page is in lru
            if(mode == "debug") cout << "page found: " << pageTable[i].address << " " << pageTable[i].rw << endl;
            if(mode == "debug") cout << "Moving " << pageTable[i].address << " " << pageTable[i].rw << endl;
            move_front(deq, pageTable[i]);
            if(pageTable[i].rw == 'W'){
               if(mode == "debug") cout << "Updating rw of: " << pageTable[i].address << endl;
                updateWrite(deq, pageTable[i]);
            }
        }
        else if(pageFound(deq, pageTable[i]) == false){ //page is not in lru
            if(mode == "debug") cout << pageTable[i].address  << " " << pageTable[i].rw << " is not in lru" << endl;
            if(deq.size() < nframes){      //lru not full
                if(mode == "debug") cout << "lru is not full" << endl;
                reads++;
                if(mode == "debug") cout << "pushing to front: " << pageTable[i].address << " " << pageTable[i].rw  << endl;
                deq.push_front(pageTable[i]); //adding new page to the front of lru
            }
            else if(deq.size() == nframes){  //lru full
                reads++;
                pageTableEntry victim = deq.back(); //eject the oldest page
                if(victim.rw == 'W'){
                    if(mode == "debug") cout << "Ejecting oldest page " << victim.address << " " << victim.rw << endl;
                    if(mode == "debug") cout << "Incrementing writes..." << endl;
                    writes++;
                }
                if(mode == "debug") cout << "Ejecting oldest page " << victim.address << " " << victim.rw << endl;
                deq.pop_back();
                if(mode == "debug") cout << "Pushing page to front: " << pageTable[i].address << " " << pageTable[i].rw  << endl;
                deq.push_front(pageTable[i]); 
            }        
        }
    }  
}
























// void fifo(vector<pageTableEntry> pageTable, int nframes){
// 	deque <pageTableEntry> deq;
//     for(int i = 0; i < pageTable.size(); i++){
//         printDeq(deq);
//         if(pageFound(deq, pageTable[i]) == true){   //page is in fifo 
//             if(pageTable[i].rw == 'W')
//                 updateWrite(deq, pageTable[i]);
//         }
//         else if(pageFound(deq, pageTable[i]) == false){ //page is not in fifo
//             if(deq.size() < nframes){      //fifo not full 
//                 reads++;
//                 deq.push_back(pageTable[i]);
//             }    
//             else if(deq.size() >= nframes){                          //fifo full
//                 reads++;
//                 pageTableEntry victim = deq.front();
//                 if(victim.dirty == true){
//                     cout << victim.address << " " << victim.rw << " was dirty ... incrementing writes" << endl;
//                     writes++;
//                 }
//                 deq.pop_front();
//                 deq.push_back(pageTable[i]);
//             }
//         }
//     }
// }





















// void fifo(vector<pageTableEntry> pageTable, int nframes){
// 	deque <pageTableEntry> deq;
//     for(int i = 0; i < pageTable.size(); i++){
//         printDeq(deq);
//         if(pageFound(deq, pageTable[i]) == true){   //page is in fifo 
//             if(pageTable[i].rw == 'W')
//                 updateWrite(deq, pageTable[i]);
//         }
//         else if(pageFound(deq, pageTable[i]) == false){ //page is not in fifo
//             if(deq.size() < nframes){      //fifo not full 
//                 reads++;
//                 deq.push_back(pageTable[i]);
//             }    
//             else if(deq.size() >= nframes){                          //fifo full
//                 reads++;
//                 pageTableEntry victim = deq.front();
//                 if(victim.dirty == true){
//                     cout << victim.address << " " << victim.rw << " was dirty ... incrementing writes" << endl;
//                     writes++;
//                 }
//                 deq.pop_front();
//                 deq.push_back(pageTable[i]);
//             }
//         }
//     }
// }





















// void fifo(vector<pageTableEntry> pageTable, int nframes){
// 	deque<pageTableEntry> deq;
//     bool pagePresent = false;
// 	for (int i=0; i<pageTable.size(); i++){ 
// 		if(find(deq.begin(), deq.end(), pageTable[i]) == deq.end()){
//             reads++;
//             deq.push_back(pageTable[i]);
//             if (deq.size() > nframes) {
//                 pageTableEntry temp2 = deq.front();
//                 if (temp2.dirty == true) {
//                     writes++; 
//                 }
//                 deq.pop_front();
//             }      	
//         }
// 		else{                                             //if the frame table is full
// 			deque < pageTableEntry > ::iterator up = (find(deq.begin(), deq.end(), pageTable[i]));         //the desired page is not in the full frame table
//             if(pageTable[i].rw == 'W'){
//                 (*up).dirty = true;
//             }                                            //if the page is not in frame table then we are adding it and regaurdless of it is a W or R we still count a read
				
//         }
            
		
// 	}
// }








































// void fifo(vector<pageTableEntry> pageTable, int nframes){
// 	unordered_set<int> frameTable;
// 	queue<int> q;
// 	for (int i=0; i<1000000; i++){ 
// 		if (frameTable.size() < nframes){ //if the frameTable is not fulll
// 			if (frameTable.find(pageTable[i].address)==frameTable.end()){ //if the desired page is not in the frame table
// 				if(pageTable[i].rw == 'R'){//read not in the frame table .... count as read and load in page table
//                     reads++; //incrementing global variable reads
// 					frameTable.insert(pageTable[i].address); //inserting into frameTable
// 					q.push(pageTable[i].address);
//                     pageTable[i].loaded = 1;
// 				}
// 				else if(pageTable[i].rw == 'W'){//write not in the frame table ... count as ready , mark as dirty , load in the page table
// 					reads++; //incrementing global variable reads
// 					pageTable[i].dirty = 1; //marking as dirty
// 					frameTable.insert(pageTable[i].address); //inserting into frameTable
// 					q.push(pageTable[i].address); //pushing to q
//                     pageTable[i].loaded = 1;
// 				}
// 			}
//             else{//frameTable is not full and desired page is in the frame table
//                 if(pageTable[i].rw == 'W'){
//                     pageTable[i].dirty = 1;
//                     pageTable[i].loaded = 1;
//                 }
//             } 
// 		}
// 		else{ //if the frame table is full
// 			if (frameTable.find(pageTable[i].address) == frameTable.end()){ //the desired page is not in the full frame table
// 				reads++; //if the page is not in frame table then we are adding it and regaurdless of it is a W or R we still count a read
// 				int val = q.front(); //val is = to the front of the queue

// 				//val is the address that needs to be evicted
// 				//	if it was dirty then count it as a write 
//                 //going through the pageTable 
//                 //  if the matching address to val is also dirty count as write
// 				for(int k = 0; k < i; k++){
//                     if(pageTable[k].address == val && pageTable[k].dirty == 1 && pageTable[k].loaded == 1){
//                         pageTable[k].loaded = 0;
//                          writes++;
//                          k = 1000000;
//                     }
//                     else if(pageTable[k].address == val && pageTable[k].dirty == 0 && pageTable[k].loaded == 1){
//                         pageTable[k].loaded = 0;
//                         k = 1000000;
//                     }
                        

//                 }
// 				q.pop(); //removing the page at the front of the queue
// 				frameTable.erase(val); //removing page that was located at the front of the q from the page table
// 				frameTable.insert(pageTable[i].address); //inserting the new desired page into the newest spot in the frameTable
//                 pageTable[i].loaded = 1;
// 				q.push(pageTable[i].address); //inserting the new desired page into the back of the queue
// 			}
//             else{//frameTable is full and desired page is in the frame table
//                 if(pageTable[i].rw == 'W'){
//                     pageTable[i].dirty = 1;
//                     pageTable[i].loaded = 1;
//                 }
//             } 
// 		}
// 	}
// }

















// void fifo(vector<pageTableEntry> pageTable, int nframes){
// 	vector<pageTableEntry> frameTable;
// 	queue<int> q;
//     int inTable = 0;
// 	for (int i=0; i<1000000; i++){
//         for(int j = 0; j < frameTable.size(); j++){//traverse frame table
//             if(frameTable[j].address == pageTable[i].address){//page is in frame table
//                 if(pageTable[i].rw == 'W'){
//                     frameTable[j].dirty == 1;
//                 }
//                 inTable = 1;
//             }
//             else{
//                 inTable = 0;
//             }
//         }
//         if(frameTable.size() < nframes && inTable == 0){//page is not in frame table and frame table is not full
//             if(pageTable[i].rw == 'R'){//read not in the frame table .... count as read and load in page table
//                 reads++; //incrementing global variable reads
//                 frameTable.push_back(pageTable[i]); //inserting into frameTable
//                 q.push(pageTable[i].address); //pushing new address onto q 
// 			}
// 			else if(pageTable[i].rw == 'W'){//write not in the frame table ... count as ready , mark as dirty , load in the page table
//                 reads++; //incrementing global variable reads
//                 pageTable[i].dirty = 1; //marking as dirty
//                 frameTable.push_back(pageTable[i]);//inserting into frameTable
//                 for(int j = 0; j < frameTable.size(); j++){//traverse frame table
//             if(frameTable[j].address == pageTable[i].address){//page is in frame table
//                 if(pageTable[i].rw == 'W'){
//                     frameTable[j].dirty == 1;
//                 }
//                 q.push(pageTable[i].address); //pushing to q
// 			}
//         }
//         else if(frameTable.size() == nframes && inTable == 0){//page is not in frame table and frame tabel is full
//             //Eject the front of FIFO and add new page to back
//             reads++;
//             int val = q.front();
//             //traverse frame table and check if address and val match
//             //  if so then check if it is a dirty bit
//             //      if dirty remove, increment write, end loop
//             //      else !dirty remove, end loop
//             for(int u = 0; u < frameTable.size(); u++){ 
//                 if(frameTable[u].address == val && frameTable[u].dirty == 1){
//                     writes++;
//                     frameTable.erase(frameTable.begin() + u);
//                     u = frameTable.size();
//                 }
//                 else if(frameTable[u].address == val && frameTable[u].dirty == 0){
//                     frameTable.erase(frameTable.begin() + u);
//                     u = frameTable.size();
//                 }

//             }
//             q.pop(); //removing the page at the front of the queue
//             if(pageTable[i].rw == 'W'){//write not in the frame table ... count as ready , mark as dirty , load in the page table
//                 pageTable[i].dirty = 1; //marking as dirty
//                 frameTable.push_back(pageTable[i]);//inserting into frameTable
//                 q.push(pageTable[i].address); //pushing to q
// 			}
//         }
//     }
// }
         


// void fifo(vector<pageTableEntry> pageTable, int nframes){
// 	vector<pageTableEntry> frameTable;
// 	queue<int> q;
//     int inTable = 0;
// 	for (int i=0; i<1000000; i++){ 
// 		if (frameTable.size() < nframes){ //if the frameTable is not fulll
//             for(int j = 0; j < frameTable.size(); j++){//traverse frame table
//                 if(frameTable[j].address == pageTable[i].address) inTable = 1;
//                 else inTable = 0;
//             }
//             if (inTable == 0){ //if the desired page is not in the frame table
//                 if(pageTable[i].rw == 'R'){//read not in the frame table .... count as read and load in page table
//                     reads++; //incrementing global variable reads
// 					frameTable.push_back(pageTable[i]); //inserting into frameTable
// 					q.push(pageTable[i].address);
//                     //pageTable[i].loaded = 1;
// 				}
// 				else if(pageTable[i].rw == 'W'){//write not in the frame table ... count as ready , mark as dirty , load in the page table
// 					reads++; //incrementing global variable reads
// 					pageTable[i].dirty = 1; //marking as dirty
// 					frameTable.push_back(pageTable[i]); //inserting into frameTable
// 					q.push(pageTable[i].address); //pushing to q
//                     //pageTable[i].loaded = 1;
// 				}
// 			}
//             else{//frameTable is not full and desired page is in the frame table
//                 if(pageTable[i].rw == 'W'){
//                     for(int x = 0; x < frameTable.size(); x++){
//                         if(pageTable[i].address == frameTable[x].address){
//                             frameTable[x].dirty = 1;
//                             //pageTable[i].loaded = 1;
//                         }
//                     }
//                 }
//             } 
// 		}
// 		else{ //if the frame table is full
// 			if (frameTable.size() == nframes && inTable == 0){ //the desired page is not in the full frame table
// 				reads++; //if the page is not in frame table then we are adding it and regaurdless of it is a W or R we still count a read
// 				int val = q.front(); //val is = to the front of the queue

// 				//val is the address that needs to be evicted
// 				//	if it was dirty then count it as a write 
//                 //going through the pageTable 
//                 //  if the matching address to val is also dirty count as write
// 				for(int u = 0; u < frameTable.size(); u++){ 
//                     if(frameTable[u].address == val && frameTable[u].dirty == 1){
//                         writes++;
//                         frameTable.erase(frameTable.begin() + u);
//                         u = frameTable.size();
//                     }
//                     else if(frameTable[u].address == val && frameTable[u].dirty == 0){
//                         frameTable.erase(frameTable.begin() + u);
//                         u = frameTable.size();
//                     }

//                 }
// 				q.pop(); //removing the page at the front of the queue
// 				frameTable.push_back(pageTable[i]); //inserting the new desired page into the newest spot in the frameTable
//                 //pageTable[i].loaded = 1;
// 				q.push(pageTable[i].address); //inserting the new desired page into the back of the queue
// 			}
//             else{//frameTable is full and desired page is in the frame table
//                 if(pageTable[i].rw == 'W'){
//                     for(int x = 0; x < frameTable.size(); x++){
//                         if(pageTable[i].address == frameTable[x].address){
//                             frameTable[x].dirty = 1;
//                             //pageTable[i].loaded = 1;
//                         }
//                     }
//                 }
//             } 
// 		}
// 	}
// }

























































