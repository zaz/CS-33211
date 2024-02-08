# compiler flags
flags = -Wall -Wextra

# "all", "clean" and "test" are not files
.PHONY: all clean test

# default action
all: producer consumer

# compile producer
producer: producer.c
	gcc $(flags) -o producer producer.c

consumer: consumer.c
	gcc $(flags) -o consumer consumer.c

clean:
	rm -f producer consumer

# compile and run producer
test: producer consumer
	./producer &
	./consumer &
