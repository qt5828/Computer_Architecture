#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // for getopt()

#define BYTES_PER_WORD 4
// #define DEBUG

/*
 * Cache structures
 */
int time = 0;

typedef struct
{
    int age;
    int valid; 
    int modified;   //아마 "dirty" bit-> update되면 1, 아니면 0
    uint32_t tag;
} cline;

typedef struct
{
    cline *lines;
} cset;

typedef struct
{
    int s; // index bits
    int E; // way
    int b; // block offset bits
    cset *sets;
} cache;

//count of the number of bits
int index_bit(int n){
    int cnt = 0;

    while(n) {
        cnt++;
        n = n >> 1;
    }

    return cnt-1;
}

/***************************************************************/
/*                                                             */
/* Procedure : build_cache                                     */
/*                                                             */
/* Purpose   : Initialize cache structure                      */
/*                                                             */
/* Parameters:                                                 */
/*     int S: The set of cache                                 */
/*     int E: The associativity way of cache                   */
/*     int b: The blocksize of cache                           */
/*                                                             */
/***************************************************************/
cache build_cache(int S, int E, int b)
{
	/* Implement this function */
	// return result_cache;
    cache temp;
    
    //s=index bit, E=way, b=block offset bit
    //capacity=1024, way=8, blocksize=8
    temp.s = index_bit(S);
    temp.E = E;
    temp.b = 3;
    temp.sets = (cset*)malloc(S*sizeof(cset));
    //initialize
    for(int i=0;i<S;i++){ 
        temp.sets[i].lines=(cline*)malloc(E*sizeof(cline));
        for(int j=0;j<E;j++){
            temp.sets[i].lines[j].age=0;
            temp.sets[i].lines[j].modified=0;
            temp.sets[i].lines[j].tag=0;
            temp.sets[i].lines[j].valid=0;
        }
    }
    return temp;
}

/***************************************************************/
/*                                                             */
/* Procedure : access_cache                                    */
/*                                                             */
/* Purpose   : Update cache stat and content                   */
/*                                                             */
/* Parameters:                                                 */
/*     cache *L: An accessed cache                             */
/*     int op: Read/Write operation                            */
/*     uint32_t addr: The address of memory access             */
/*     int *hit: The number of cache hit                       */
/*     int *miss: The number of cache miss                     */
/*     int *wb: The number of write-back                       */
/*                                                             */
/***************************************************************/
void access_cache(cache *L, int op, uint32_t addr, int *hit, int *miss, int *wb)
{
	/* Implement this function */

    //S=index bit, E=way, B=block offset bit
    int S = L->s, E = L->E, B = L->b;
    int old = 0;

    //lecture note : chap12 p.13
    //tag = addr>>(index bit + block offset bit)
    //index = ( add>>(block offset bit) ) & ( 2^(index bit)-1 )
    uint32_t tag = addr>>(S+B);
    cline *index = L->sets[(addr>>B)&((1<<S)-1)].lines;
    //L->sets[(addr>>B)&((1<<S)-1)].lines = index;

    //Read
    if (op=='R'){
        for(int i=0; i<E; i++){
            //miss(empty) => write data/tag proper locations
            if(L->sets[(addr>>B)&((1<<S)-1)].lines[i].valid == 0){
                index[i].age = time;
                index[i].valid = 1;
                index[i].tag = tag;
                index[i].modified = 0;
                (*miss)++;

                //update L
                L->sets[(addr>>B)&((1<<S)-1)].lines = index;
                return;
            }
            else{
                //hit
                if(index[i].tag == tag){
                    index[i].age = time;
                    index[i].tag = tag;
                    (*hit)++;
                    
                    //update L
                    L->sets[(addr>>B)&((1<<S)-1)].lines = index;
                    return;
                }
                //not hit
                //compare(find) oldest data in cache
                else{
                    if(index[i].age < index[old].age)
                        old = i;
                }
            }
        }
        //miss=>write data/tag into proper locations
        (*miss)++;

        //replace dirty block => writeback
        if(index[old].modified == 1){
            index[old].age = time;
            index[old].tag = tag;
            index[old].modified = 0;
            (*wb)++;
        }
        //replace clean block => no extra traffic
        else{
            index[old].age = time;
            index[old].tag = tag;
        }

        //update L
        L->sets[(addr>>B)&((1<<S)-1)].lines = index;
        return;
    }


    //Write
    else{
        for(int i=0;i<E;i++){
            //miss(empty)=>writes data/tag into proper locations
            if(index[i].valid == 0){
                index[i].age = time; 
                index[i].valid = 1;
                index[i].tag = tag;
                //modified!
                index[i].modified = 1;
                (*miss)++;

                //update L
                L->sets[(addr>>B)&((1<<S)-1)].lines = index;
                return;
            }
            else{
                //hit
                if(index[i].tag == tag){
                    index[i].age = time;
                    index[i].tag = tag;
                    //modified!
                    index[i].modified = 1;
                    (*hit)++;

                    //update L
                    L->sets[(addr>>B)&((1<<S)-1)].lines = index;
                    return;
                }
                //not hit
                //compare(find) oldest data in cache
                else{
                    if(index[i].age < index[old].age)
                        old = i;
                }
            }
        }
        //miss=>writes data/tag into proper locations
        (*miss)++;

        //replace dirty block => writeback
        if(index[old].modified == 1){
            index[old].age = time;
            index[old].tag = tag;
            (*wb)++;
            //modified! => still modified = 1.
        }
        //replace clean block => no extra traffic
        else{
            index[old].age = time;
            index[old].tag = tag;
            //modified!
            index[old].modified = 1;
        }

        //update L
        L->sets[(addr>>B)&((1<<S)-1)].lines = index;
        return;
    }
}

/***************************************************************/
/*                                                             */
/* Procedure : cdump                                           */
/*                                                             */
/* Purpose   : Dump cache configuration                        */
/*                                                             */
/***************************************************************/
void cdump(int capacity, int assoc, int blocksize)
{

    printf("Cache Configuration:\n");
    printf("-------------------------------------\n");
    printf("Capacity: %dB\n", capacity);
    printf("Associativity: %dway\n", assoc);
    printf("Block Size: %dB\n", blocksize);
    printf("\n");
}

/***************************************************************/
/*                                                             */
/* Procedure : sdump                                           */
/*                                                             */
/* Purpose   : Dump cache stat                                 */
/*                                                             */
/***************************************************************/
void sdump(int total_reads, int total_writes, int write_backs,
           int reads_hits, int write_hits, int reads_misses, int write_misses)
{
    printf("Cache Stat:\n");
    printf("-------------------------------------\n");
    printf("Total reads: %d\n", total_reads);
    printf("Total writes: %d\n", total_writes);
    printf("Write-backs: %d\n", write_backs);
    printf("Read hits: %d\n", reads_hits);
    printf("Write hits: %d\n", write_hits);
    printf("Read misses: %d\n", reads_misses);
    printf("Write misses: %d\n", write_misses);
    printf("\n");
}


/***************************************************************/
/*                                                             */
/* Procedure : xdump                                           */
/*                                                             */
/* Purpose   : Dump current cache state                        */
/*                                                             */
/* Cache Design                                                */
/*                                                             */
/*      cache[set][assoc][word per block]                      */
/*                                                             */
/*                                                             */
/*       ----------------------------------------              */
/*       I        I  way0  I  way1  I  way2  I                 */
/*       ----------------------------------------              */
/*       I        I  word0 I  word0 I  word0 I                 */
/*       I  set0  I  word1 I  word1 I  work1 I                 */
/*       I        I  word2 I  word2 I  word2 I                 */
/*       I        I  word3 I  word3 I  word3 I                 */
/*       ----------------------------------------              */
/*       I        I  word0 I  word0 I  word0 I                 */
/*       I  set1  I  word1 I  word1 I  work1 I                 */
/*       I        I  word2 I  word2 I  word2 I                 */
/*       I        I  word3 I  word3 I  word3 I                 */
/*       ----------------------------------------              */
/*                                                             */
/*                                                             */
/***************************************************************/
void xdump(cache* L)
{
    int i, j, k = 0;
    int b = L->b, s = L->s;
    int way = L->E, set = 1 << s;

    uint32_t line;

    printf("Cache Content:\n");
    printf("-------------------------------------\n");

    for(i = 0; i < way; i++) {
        if(i == 0) {
            printf("    ");
        }
        printf("      WAY[%d]", i);
    }
    printf("\n");

    for(i = 0; i < set; i++) {
        printf("SET[%d]:   ", i);

        for(j = 0; j < way; j++) {
            if(k != 0 && j == 0) {
                printf("          ");
            }
            if(L->sets[i].lines[j].valid) {
                line = L->sets[i].lines[j].tag << (s + b);
                line = line | (i << b);
            }
            else {
                line = 0;
            }
            printf("0x%08x  ", line);
        }
        printf("\n");
    }
    printf("\n");
}


int main(int argc, char *argv[])
{
    int capacity=1024;
    int way=8;
    int blocksize=8;
    int set;

    // Cache
    cache simCache;

    // Counts
    int read=0, write=0, writeback=0;
    int readhit=0, writehit=0;
    int readmiss=0, writemiss = 0;

    // Input option
    int opt = 0;
    char* token;
    int xflag = 0;

    // Parse file
    char *trace_name = (char*)malloc(32);
    FILE *fp;
    char line[16];
    char *op;
    uint32_t addr;

    /* You can define any variables that you want */

    trace_name = argv[argc-1];
    if (argc < 3) {
        printf("Usage: %s -c cap:assoc:block_size [-x] input_trace \n", argv[0]);
        exit(1);
    }

    while((opt = getopt(argc, argv, "c:x")) != -1) {
        switch(opt) {
            case 'c':
                token = strtok(optarg, ":");
                capacity = atoi(token);
                token = strtok(NULL, ":");
                way = atoi(token);
                token = strtok(NULL, ":");
                blocksize  = atoi(token);
                break;

            case 'x':
                xflag = 1;
                break;

            default:
                printf("Usage: %s -c cap:assoc:block_size [-x] input_trace \n", argv[0]);
                exit(1);

        }
    }

    // Allocate
    set = capacity / way / blocksize;

    /* TODO: Define a cache based on the struct declaration */
    simCache = build_cache(set,way,blocksize);

    // Simulate
    fp = fopen(trace_name, "r"); // read trace file
    if(fp == NULL) {
        printf("\nInvalid trace file: %s\n", trace_name);
        return 1;
    }

    cdump(capacity, way, blocksize);
    
    /* TODO: Build an access function to load and store data from the file */
    time = 0;
    while (fgets(line, sizeof(line), fp) != NULL) {
        op = strtok(line, " ");
        addr = strtoull(strtok(NULL, ","), NULL, 16);

#ifdef DEBUG
        // You can use #define DEBUG above for seeing traces of the file.
        fprintf(stderr, "op: %s\n", op);
        fprintf(stderr, "addr: %x\n", addr);
#endif
        // access_cache()
        
        //READ
        if (strcmp(op,"R") == 0){
            access_cache(&simCache,'R',addr,&readhit,&readmiss,&writeback);
            read++;
        }
        //WRITE
        else{
            access_cache(&simCache,'W',addr,&writehit,&writemiss,&writeback);
            write++;
        }
        time++;
    }

    // test example
    sdump(read, write, writeback, readhit, writehit, readmiss, writemiss);
    if (xflag) {
        xdump(&simCache);
    }

    return 0;
}
