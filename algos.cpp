#include "algos.h"
#include<bits/stdc++.h>
#include <iostream>
#include <vector>
using namespace std;


void fifo(vector<pageTableEntry> pageTable, int nframes){
	unordered_set<int> frameTable;
	queue<int> q;
	for (int i=0; i<1048576; i++){ 
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
					q.push(pageTable[i].address);
				}
			}
		}
		else{ //if the frame table is full
			if (frameTable.find(pageTable[i].address) == frameTable.end()){ //the desired page is not in the full frame table
				reads++; //if the page is not in frame table then we are adding it and regaurdless of it is a W or R we still count a read
				int val = q.front(); //val is = to the front of the queue
				//find the corresponding page that contains the address of val and check if it was dirty
				//	if it was dirty then count it as a write 
				for(int i = 0; i < 1048576; i++){
					if(pageTable[i].address == val && pageTable[i].dirty == 1)
						writes++;
				} 
				q.pop(); //removing the page at the front of the queue
				frameTable.erase(val); //removing page that was located at the front of the q from the page table
				frameTable.insert(pageTable[i].address); //inserting the new desired page into the newest spot in the frameTable
				q.push(pageTable[i].address); //inserting the new desired page into the back of the queue
			}
		}
	}
}

// void lru(vector<pageTableEntry> pageTable, int nframes){

// }

// void vms(vector<pageTableEntry> pageTable, int nframes, int p){

// }
