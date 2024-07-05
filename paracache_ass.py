#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Cache parameters
#define CACHE_SIZE 256      // Size of the Cache in bytes
#define BLOCK_SIZE 32        // Size of each block in bytes
#define NUM_BLOCKS (CACHE_SIZE / BLOCK_SIZE) // Number of blocks in the cache

// Memory parameters
#define MEMORY_SIZE 65536    // Size of main memory in bytes

typedef struct {
    int valid;
    unsigned int tag;
    unsigned int data; // Start address of the block in memory
} CacheBlock;

CacheBlock cache[NUM_BLOCKS];

// Function to extract tag and index from an address
void getAddressComponents(unsigned int address, unsigned int* tag, unsigned int* index) {
    unsigned int offsetBits = (unsigned int)log2(BLOCK_SIZE);
    unsigned int indexBits = (unsigned int)log2(NUM_BLOCKS);
    unsigned int tagBits = 32 - offsetBits - indexBits;
    *index = (address >> offsetBits) & ((1 << indexBits) - 1);
    *tag = (address >> (offsetBits + indexBits)) & ((1 << tagBits) - 1);
}

// Function to print the state of the full cache
void showCacheView() {
    printf("Cache View:\n");
    for (int i = 0; i < NUM_BLOCKS; i++) {
        if (cache[i].valid) {
            unsigned int start = cache[i].data - cache[i].data % BLOCK_SIZE;
            unsigned int end = start + BLOCK_SIZE - 1;
            printf("Block %d: Valid = %d, Data Range = [%u - %u]\n", i, cache[i].valid, start, end);
        }
        else {
            printf("Block %d: Valid = %d, Data Range = Empty\n", i, cache[i].valid);
        }
    }
}

// Cache simulation function
void accessCache(unsigned int address, int* hits, int* misses, int* evictions) {
    unsigned int tag, index;
    getAddressComponents(address, &tag, &index);
    if (cache[index].valid && cache[index].tag == tag) {
        // Cache hit
        printf("hit\n");
        (*hits)++;
    }
    else {
        // Cache miss
        printf("miss\n");
        (*misses)++;
        if (cache[index].valid) {
            // Eviction needed
            (*evictions)++;
            printf("swap\n");
        }
        cache[index].valid = 1;
        cache[index].tag = tag;
        cache[index].data = address; // Set the data field to the address
    }
    // Print the state of the cache after each access
    showCacheView();
}

int main() {
    // Initialize cache
    for (int i = 0; i < NUM_BLOCKS; i++) {
        cache[i].valid = 0;
    }
    unsigned int address;
    int hits = 0, misses = 0, evictions = 0;
    printf("Enter memory addresses (0 to stop): \n");
    while (1) {
        scanf("%u", &address);
        if (address == 0) {
            break;
        }
        accessCache(address, &hits, &misses, &evictions);
    }
    printf("Total hits: %d\n", hits);
    printf("Total misses: %d\n", misses);
    double hitrate = (double)hits / (hits + misses);
    double missrate = 1 - hitrate;
    printf("Hit Rate: %f%%\n", hitrate*100);
    printf("Miss Rate: %f%%\n", missrate*100);
    return 0;
}