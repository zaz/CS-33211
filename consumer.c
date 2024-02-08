#include <sys/mman.h>  // include shared memory for shm_open
#include <fcntl.h>     // for O_* constants
#include <stdio.h>     // for printf
#include <unistd.h>    // for ftruncate

// TODO: security: restrict syscalls with SECCOMP

// Modifiable constants
const char *sharedMemoryName = "/producerConsumerTable";
const unsigned int nSlots = 2;    // number of slots on the table
const unsigned int itemSize = 4;  // 1 word (32 bits) per item

// Derived constants
const unsigned int tableSize = nSlots * itemSize;


// TODO: move this into its own header
unsigned int* initializeSharedMemory(
    const char *sharedMemoryName, const unsigned int tableSize) {
    int fd = shm_open(sharedMemoryName, O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open");
        return NULL;
    }
    if (ftruncate(fd, tableSize) == -1) {
        perror("ftruncate");
        return NULL;
    }
    void *ptr = mmap(NULL, tableSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        return NULL;
    }
    return (unsigned int*) ptr;
}

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
