
.RECIPEPREFIX = >
.PHONY: clean
CC = gcc
OO = -g -Og
FFLAGS = -fstack-protector-all -fno-omit-frame-pointer
MFLAGS = -mincoming-stack-boundary=3
WFLAGS = -Wall -Wextra -Wstack-protector
CFLAGS = $(FLAGS) $(OO) $(FFLAGS) $(MFLAGS) $(WFLAGS)
SRC = ./src
LSRC = $(SRC)/lib
TEST = ./test
LTEST = $(TEST)/lib
CCOBJ = $(CC) -o $@ $^
NAME = kpl
TNAME = _test
LTNAME = _lib$(TNAME)

all: $(NAME)

LJIT_OBJS = $(LSRC)/jit.o
jit$(LTNAME): $(LJIT_OBJS) $(LTEST)/jit.o
> $(CCOBJ)

LTE_OBJS = $(LSRC)/te.o
te$(LTNAME): $(LTE_OBJS) $(LTEST)/te.o
> $(CCOBJ)

LVR_OBS = $(LSRC)/vr.o
vr$(LTNAME): $(LVR_OBS) $(LTEST)/vr.o
> $(CCOBJ)

LLST_OBJS = $(LSRC)/lst.o $(LTE_OBJS)
lst$(LTNAME): $(LLST_OBJS) $(LTEST)/lst.o
> $(CCOBJ)

TTBL_OBJS = $(LSRC)/tbl.o $(LLST_OBJS)
tbl$(LTNAME): $(LTBL_OBJS) $(LTEST)/tbl.o
> $(CCOBJ)

clean:
> rm -fv $(SRC)/**/*.o $(TEST)/**/*.o $(NAME) *$(TNAME)
