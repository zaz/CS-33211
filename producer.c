#include <sys/mman.h>  // include shared memory for shm_open
#include <fcntl.h>     // for O_* constants
#include <stdio.h>     // for printf
#include <unistd.h>    // for ftruncate

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
    *p = 0xdeadbeef;
    printf("producer: %d\n", *p);
    return 0;
}
