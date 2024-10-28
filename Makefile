CC=gcc
CFLAGS= -Wall -Wextra -Werror -g -fsanitize=address
OBJS=rbt.c main.c 
DEPS=rbt.h 

all: $(DEPS)
	$(CC) $(CFLAGS) $(OBJS) -o koala