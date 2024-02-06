# Proof of Concept of Using Shared Memory

This program serves as a proof of concept of using shared memory.

## Quick Start Guide

Run `make test` to build and run the program.

To watch the shared memory change as the program runs, run:

```bash
watch -n .2 xxd -e /dev/shm/producerConsumerTable
```

