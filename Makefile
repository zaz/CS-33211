# compiler flags
flags = -Wall -Wextra

# "all", "clean" and "test" are not files
.PHONY: all clean test

# default action
all: producer

# compile producer
producer: producer.c
	gcc $(flags) -o producer producer.c

clean:
	rm -f producer

# compile and run producer
test: producer
	./producer
