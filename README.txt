Authors: Eric Kemmer, Randall Hunt 
This program is for COP 4600 Operating Systems - Memory Virtualization

To run the following program: 
1. Go to the directly that contains the files. 
2. Type the following into the command line and press enter: "make" 
3. Run the program by typing this following into the command line:
    "./memsim <tracefile> <number of frames> <fifo | lru | vms> <percent only for vms> <quiet | debug> 

            Notes: 1. Above you will enter the full name for the trace file that is to be used. 
                    2. Enter the number of frames you wish to use. 
                     3. Enter one of the three algorithms provided, FIFO ( first in first out), LRU (Least Recently Used), VMS (Segmented FIFO);
                      4. The percentage for vms is to be an interger between 1-100 and is ONLY used for vms. 
                       5. Enter wether you want the program to run quiet or debug. 

    Examples: 
        ./memsim bzip.trace 64 fifo quiet 
        ./memsim bzip.trace 64 lru quiet 
        ./memsim sixpack.trace 64 vms 30 quiet
        ./memsim sixpack.trace 64 vms 40 debug  
