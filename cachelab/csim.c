#include <getopt.h>
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cachelab.h"

// cache parameters
int s, S, E, b;
int set_mask;
int trace = 0;

// results
int hits = 0;
int misses = 0;
int evictions = 0;

// global time
int tik = 0;

// cache definition
typedef struct {
    int valid;
    int tag;
    int timestamp;
} CacheLine, *CacheSet, **Cache;

// cache instance
Cache cache = NULL;

// functions
void buildCache();
void freeCache();
void updateCache(unsigned long address);
void process(char* file);
void printLog(const char *fmt, ...);
void printUsage();

int main(int argc, char* argv[])
{
    char c, file[128];
    while ((c = getopt(argc, argv, "s:E:b:t:hv")) != -1) {
        switch (c) {
        case 's':
            s = atoi(optarg);
            break;
        case 'E':
            E = atoi(optarg);
            break;
        case 'b':
            b = atoi(optarg);
            break;
        case 't':
            strcpy(file, optarg);
            break;
        case 'v':
            trace = 1;
            break;
        case 'h':
            printUsage();
            exit(0);
        default:
            printf("Unrecognized option: -%c", c);
            exit(1);
        }
    }

    if (s < 0 || E <= 0 || b <= 0) {
        printf("Error: Invalid s, E, b parameters");
        exit(1);
    }

    buildCache();
    process(file);
    freeCache();

    printSummary(hits, misses, evictions);
    return 0;
}

void buildCache() {
    S = 1 << s;
    cache = (Cache) malloc(sizeof(CacheSet) * S);
    int set_size = sizeof(CacheLine) * E;
    for (int i = 0; i < S; ++i) {
        cache[i] = (CacheSet) malloc(set_size);
        memset(cache[i], 0, sizeof(set_size));
    }
    set_mask = S - 1;
}

void freeCache() {
    for (int i = 0; i < S; ++i) {
        free(cache[i]);
    }
    free(cache);
    cache = NULL;
}

void process(char* file) {
    FILE* fp = fopen(file, "r");
    char op;
    unsigned int size;
    unsigned long address;
    while (fscanf(fp, " %c %lx,%u", &op, &address, &size) == 3) {
        printLog("%c %lx,%u ", op, address, size);
        switch (op) {
        case 'M':
            updateCache(address); // M = L + S;
        case 'L':
        case 'S':
            updateCache(address);
            break;
        default:
            ;// do nothing
        }
        printLog("\n");
    }
}

void updateCache(unsigned long address) {
    tik++;
    address >>= b;
    int tag = address >> s;
    int set_index = address & set_mask;
    CacheSet set = cache[set_index];

    for (int i = 0; i < E; ++i) {
        if (set[i].valid && set[i].tag == tag) {
            hits++;
            set[i].timestamp = tik;
            printLog("hit ");
            return;
        }
    }

    misses++;
    printLog("miss ");

    int min_time = INT_MAX;
    int min_index = -1;
    for (int i = 0; i < E; ++i) {
        if (!set[i].valid) {
            set[i].valid = 1;
            set[i].tag = tag;
            set[i].timestamp = tik;
            return;
        }
        if (set[i].timestamp < min_time) {
            min_time = set[i].timestamp;
            min_index = i;
        }
    }

    set[min_index].tag = tag;
    set[min_index].timestamp = tik;

    evictions++;
    printLog("eviction ");
}

void printLog(const char *fmt, ...) {
    if (trace) {
        va_list args;
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
    }
}

void printUsage() {
    printf("Usage:\n");
    printf("  -h: Optional help flag that prints usage info\n");
    printf("  -v: Optional verbose flag that displays trace info\n");
    printf("  -s <s>: Number of set index bits (S = 2^s is the number of sets)\n");
    printf("  -E <E>: Associativity (number of lines per set)\n");
    printf("  -b <b>: Number of block bits (B = 2^b is the block size)\n");
    printf("  -t <tracefile>: Name of the valgrind trace to replay\n");
}
