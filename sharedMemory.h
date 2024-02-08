#include <sys/mman.h>  // include shared memory for shm_open
#include <fcntl.h>     // for O_* constants
#include <stdio.h>     // for printf
#include <unistd.h>    // for ftruncate

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
