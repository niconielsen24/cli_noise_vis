CC = gcc
CFLAGS = -Wall -Wextra
SRCS = $(wildcard *.c)
OBJS = $(patsubst %.c,%.o,$(SRCS))
EXEC = noise

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	erase -f $(OBJS) $(EXEC).exe

run : $(EXEC)
	./$(EXEC).exe
