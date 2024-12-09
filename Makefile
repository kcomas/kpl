
.RECIPEPREFIX = >
.PHONY: clean
CC = gcc
OO = -g -Og
FFLAGS = -fstack-protector-all -fno-omit-frame-pointer
MFLAGS = -mincoming-stack-boundary=3
WFLAGS = -Wall -Wextra -Wstack-protector
CFLAGS = $(FLAGS) $(OO) $(FFLAGS) $(MFLAGS) $(WFLAGS)
SRC = ./src
TEST = ./test
SRCS = $(wildcard $(SRC)/*.c)
TSRCS = $(wildcard $(TEST)/*.c)
CCOBJ = $(CC) -o $@ $^
OBJS = $(patsubst %.c, %.o, $(SRCS))
NAME = kpl
TNAME = _test

all: $(NAME)

JIT_OBJS = $(SRC)/jit.o
jit$(TNAME): $(JIT_OBJS) $(TEST)/jit.o
> $(CCOBJ)

VR_OBS = $(SRC)/vr.o
vr$(TNAME): $(VR_OBS) $(TEST)/vr.o
> $(CCOBJ)

%.o: %.c %.h
> $(CC) $(CFLAGS) -c $< -o $@

clean:
> rm -fv $(SRC)/*.o $(TEST)/*.o $(NAME) *$(TNAME)
