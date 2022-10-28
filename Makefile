OBJS	= resvars.o hashmap.o
OBJS = $(src:.c=.o)
SRC = $(wildcard *.c)
SOURCE	= resvars.c hashmap.c
HEADER	= hashmap.h
OUT	= hashmap
CC	 = gcc
FLAGS	 = -g -c -Wall
# -g option enables debugging mode 
# -c flag generates object code for separate files
# -Wall flag enables all warnings

all: $(OBJS)
  $(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)


# create/compile the individual files

%.o:%.c
  $(CC) $(FLAGS) $< -o $@
%.o:%.c
  $(CC) $(FLAGS) $< -o $@



# clean house
clean:
  rm -f $(OBJS) $(OUT)
