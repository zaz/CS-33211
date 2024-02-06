#include <sys/mman.h>  // include shared memory for shm_open
#include <fcntl.h>     // for O_* constants
#include <stdio.h>     // for printf
#include <unistd.h>    // for ftruncate

// TODO: security: restrict syscalls with SECCOMP

// Modifiable constants
const char *sharedMemoryName = "/producerConsumerTable";
const unsigned int nItems = 2;
const unsigned int itemSize = 4;  // 1 word (32 bits) per item

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
    *p = 0xDEADBEEF;
    printf("producer: %d\n", *p);

    // create pointer to the second item
    unsigned int *q = p + 1;
    *q = 0xB0BABABE;
}

int main() {
    unsigned int *p = initializeSharedMemory(sharedMemoryName, tableSize);
    if (p == NULL) { return 1; }

    produceItems(p, nItems);

    return 0;
}

// TODO: close shared memeory on exit
