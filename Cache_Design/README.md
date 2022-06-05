# Project 3. MIPS Data Cache Simulator
Skeleton developed by CMU, modified for AJOU SCE212

More details are in [handout](./handout/Project_3.pdf)

## Instructions
All student is required to implement functions below. 
1. __build_cache()__  
    A role of this function is to allocate memory for cache structure.
    You must use the declared cache structure and arguments from the command line.
    All variables in the cache structure should be initialized and used appropriately.

2. __access_cache()__  
    If you let a system allocate the memory of cache structures,
    the next step would be reading a file and applying address lines in the file to
    the cache structures. Given those addresses, the cache structure should contain
    proper addresses in the cache line.

    
    >Note that the cache does not have any data, so you only need to consider
    the proper address value of the cache.

    For a write policy, there are two write policies in the cache:
    write-through and write-back. In this project, for simplicity,
    write-back policy is used and only counted by a variable. Hence,
    You should define a proper count variable to get the same result with the files in `sample_output` directory.

    Whenever an existing cache entry is evicted during cache miss due to the lack of room,
    there should be an appropriate replacement policy.
    In this project, we use "Least Replacement Unit" (LRU). Thus, you should be
    aware of it to make the required output result.
    
* __Sample files__  
Each sample file in `sample_input` directory has two columns. The first column stands for
a type of operations such as "R" for Read and "W" for Write. The second column is an address value.
So, you should interpret and use them properly for their function.

## ETC
The functions above can be modified if the application produces the output you do not expect.
So, feel free to change and add any functions if you desire. If you have any questions,
please post your question in the Ajou BB.
