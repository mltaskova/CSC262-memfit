Maria-Luiza Taskova P2

For the Java implementation the Simulation class holds the main function and handles the file input. Command line argument should be the input file name.
The Block class is used in the simulation to mimic memory.
The BySize, ByOffset classes are comparators.
I added the fragmentation percentage too but I had to look up how to calculate it - (current_free_space - size_of_biggest_block)/current_free_space .

I used the file input.txt in the parent directory so we can directly edit it and see the changes.

Fun fact: I ran random a couple of times with the current content of the file and it allocated memory for all blocks! (Right now all the other algorithms cannot allocate X)
