#include <sys/mman.h>  // include shared memory for shm_open
#include <fcntl.h>     // for O_* constants
#include <stdio.h>     // for printf
#include <unistd.h>    // for ftruncate

// TODO: security: restrict syscalls with SECCOMP

const unsigned int tableSize = 2 * 4;  // 2 words

int main() {
    int fd = shm_open("/producerConsumerTable", O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open");
        return 1;
    }
    if (ftruncate(fd, tableSize) == -1) {
        perror("ftruncate");
        return 1;
    }
    void *ptr = mmap(NULL, tableSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        return 1;
    }
    unsigned int *p = (unsigned int*) ptr;

    // TODO: produce random items
    *p = 0xDEADBEEF;
    printf("producer: %d\n", *p);

    // create pointer to the second item
    unsigned int *q = p + 1;
    *q = 0xB0BABABE;

    return 0;
}

// TODO: close shared memeory on exit
