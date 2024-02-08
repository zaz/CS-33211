#include <sys/mman.h>  // include shared memory for shm_open
#include <fcntl.h>     // for O_* constants
#include <stdio.h>     // for printf
#include <unistd.h>    // for ftruncate
#include "sharedMemory.h"

// TODO: security: restrict syscalls with SECCOMP

// Modifiable constants
const char *sharedMemoryName = "/producerConsumerTable";
const unsigned int nSlots = 2;    // number of slots on the table
const unsigned int itemSize = 4;  // 1 word (32 bits) per item

// Derived constants
const unsigned int tableSize = nSlots * itemSize;


void consumeItems(unsigned int *p, unsigned int nSlots) {
    while (1) {
        for (unsigned int slot = 0; slot < nSlots; ++slot) {
            if (*(p + slot) != 0) {
                printf("Table slot %d full. Consume 0x%x\n", slot, *(p + slot));
                *(p + slot) = 0;
                break;
            }
        }
        sleep(1);
    }
}

int main() {
    unsigned int *p = initializeSharedMemory(sharedMemoryName, tableSize);
    if (p == NULL) { return 1; }

    consumeItems(p, nSlots);

    return 0;
}

// TODO: close shared memeory on exit
