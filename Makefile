
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

LDEF_OBJS = $(LSRC)/def.o
def$(LTNAME): $(LDEF_OBJS) $(LTEST)/def.o
> $(CCOBJ)

LMC_OBJS = $(LSRC)/mc.o
mc$(LTNAME): $(LMC_OBJS) $(LTEST)/mc.o
> $(CCOBJ)

LX64_OBJS = $(LSRC)/x64.o
x64$(LTNAME): $(LX64_OBJS) $(LTEST)/x64.o
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

LTBL_OBJS = $(LSRC)/tbl.o $(LLST_OBJS)
tbl$(LTNAME): $(LTBL_OBJS) $(LTEST)/tbl.o
> $(CCOBJ)

LTKN_OBJS = $(LSRC)/tkn.o $(LTBL_OBJS) $(LMC_OBJS) $(LDEF_OBJS)
tkn$(LTNAME): $(LTKN_OBJS) $(LTEST)/tkn.o $(LTEST)/tkn_t.o
> $(CCOBJ)

LPSR_OBJS = $(LSRC)/psr.o $(LVR_OBS) $(LTKN_OBJS) $(LVR_OBS)
psr$(LTNAME): $(LPSR_OBJS) $(LTEST)/psr.o $(LTEST)/psr_t.o $(LTEST)/tkn_t.o
> $(CCOBJ)

LAS_OBJS = $(LSRC)/as.o $(LTBL_OBJS)
as$(LTNAME): $(LAS_OBJS) $(LTEST)/as.o $(LTEST)/as_t.o $(LX64_OBJS)
> $(CCOBJ)

clean:
> rm -fv $(SRC)/**/*.o $(TEST)/**/*.o $(NAME) *$(TNAME)
