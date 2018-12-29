CC = gcc
CFLAGS = -std=c99 -g -pedantic -Wall -Werror
TEST_FLAGS = -std=c99 -g -pedantic -Wall -Werror
OBJS = list.o atomic.o menu.o reader.o
BENCHMARK_OBJS = list.o atomic.o printer.o benchmark.o

ifeq ($(mode), debug)
	CFLAGS += -g3
endif

ifeq ($(mode), release)
	CFLAGS += -DNDEBUG -g0
endif

all: app.exe test.exe

benchmark.exe: $(BENCHMARK_OBJS)
	$(CC) -o $@ $^ -pthread

app.exe: $(OBJS) main.o
	$(CC) $(CFLAGS) -o $@ $^

test.exe: $(OBJS) test.o
	$(CC) $(TEST_FLAGS) -o $@ $^

%.o: %.c *.h
	$(CC) $(CFLAGS) -c $<

clean:
	$(RM) *.o *.exe *.h.gch
