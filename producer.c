#include <sys/mman.h>  // include shared memory for shm_open
#include <fcntl.h>     // for O_* constants
#include <stdio.h>     // for printf
#include <unistd.h>    // for ftruncate

// TODO: security: restrict syscalls with SECCOMP

// Modifiable constants
const char *sharedMemoryName = "/producerConsumerTable";
const unsigned int nItems = 2;
const unsigned int itemSize = 4;  // 1 word (32 bits) per item
// create an array of integers
const unsigned short int items[14] = {
    0xDEAD, 0xBEEF, 0xB0BA, 0xBABE, 0x0BAD, 0xCAFE, 0xF00D,
    0x0BEE, 0x0F00, 0x0DAD, 0xC0DE, 0xFACE, 0xB00B, 0x0CAB
};

// Derived constants
const unsigned int tableSize = nItems * itemSize;

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

void produceItems(unsigned int *p, unsigned int nItems) {
    // TODO: produce random items
    *p = 0xDEAD;
    printf("producer: %d\n", *p);

    // create pointer to the second item
    unsigned int *q = p + 1;
    *q = 0xBEEF;
}

int main() {
    unsigned int *p = initializeSharedMemory(sharedMemoryName, tableSize);
    if (p == NULL) { return 1; }

    produceItems(p, nItems);

    return 0;
}

// TODO: close shared memeory on exit
