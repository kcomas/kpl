
.RECIPEPREFIX = >
.PHONY: clean
CC = gcc
OO = -g -Og
FFLAGS = -fstack-protector-all -fno-omit-frame-pointer
MFLAGS = -mincoming-stack-boundary=3
WFLAGS = -Wall -Wextra -Wstack-protector
CFLAGS = -std=gnu99 $(FLAGS) $(OO) $(FFLAGS) $(MFLAGS) $(WFLAGS)
SRC = ./src
LSRC = $(SRC)/lib
TEST = ./test
LTEST = $(TEST)/lib
CCOBJ = $(CC) -o $@ $^
NAME = kpl
TNAME = _test
LTNAME = _lib$(TNAME)

all: $(NAME)

LTEST_OBJS = $(LTEST)/t.o
test$(LTNAME): $(LTEST_OBJS) $(LTEST)/test.o
> $(CCOBJ)

LDEF_OBJS = $(LSRC)/def.o
def$(LTNAME): $(LDEF_OBJS) $(LTEST)/def.o
> $(CCOBJ)

LMC_OBJS = $(LSRC)/mc.o
mc$(LTNAME): $(LMC_OBJS) $(LTEST)/mc.o $(LTEST_OBJS)
> $(CCOBJ)

LX64_OBJS = $(LSRC)/x64.o
x64$(LTNAME): $(LX64_OBJS) $(LTEST)/x64.o $(LTEST_OBJS)
> $(CCOBJ)

LTE_OBJS = $(LSRC)/te.o
te$(LTNAME): $(LTE_OBJS) $(LTEST)/te.o $(LTEST_OBJS)
> $(CCOBJ)

LVR_OBS = $(LSRC)/vr.o
vr$(LTNAME): $(LVR_OBS) $(LTEST)/vr.o $(LTEST_OBJS)
> $(CCOBJ)

LLST_OBJS = $(LSRC)/lst.o $(LTE_OBJS)
lst$(LTNAME): $(LLST_OBJS) $(LTEST)/lst.o $(LTEST_OBJS)
> $(CCOBJ)

LTBL_OBJS = $(LSRC)/tbl.o $(LLST_OBJS)
tbl$(LTNAME): $(LTBL_OBJS) $(LTEST)/tbl.o $(LTEST_OBJS)
> $(CCOBJ)

LTKN_OBJS = $(LSRC)/tkn.o $(LTBL_OBJS) $(LMC_OBJS) $(LDEF_OBJS)
tkn$(LTNAME): $(LTKN_OBJS) $(LTEST)/tkn.o $(LTEST)/tkn_t.o
> $(CCOBJ)

LPSR_OBJS = $(LSRC)/psr.o $(LVR_OBS) $(LTKN_OBJS) $(LVR_OBS)
psr$(LTNAME): $(LPSR_OBJS) $(LTEST)/psr.o $(LTEST)/psr_t.o $(LTEST)/tkn_t.o
> $(CCOBJ)

LAS_OBJS = $(LSRC)/as.o $(LTBL_OBJS)
LASX64_OBJS = $(LSRC)/as_x64.o $(LX64_OBJS)
as$(LTNAME): $(LAS_OBJS) $(LTEST)/as.o $(LTEST)/as_t.o $(LASX64_OBJS)
> $(CCOBJ)

GEN_OBJS = $(LSRC)/gen.o $(LAS_OBJS)
GENX64_OBJS = $(GEN_OBJS) $(LSRC)/gen_x64.o $(LX64_OBJS) $(LASX64_OBJS) $(LVR_OBS) $(patsubst %.c,%.o,$(wildcard $(LSRC)/gen_x64/*.c))
gen$(LTNAME): $(GENX64_OBJS) $(LTEST)/gen.o $(LTEST)/gen_t.o $(LTEST)/as_t.o
> $(CCOBJ)

clean:
> find -type f -regex "^\($(SRC)\|$(TEST)\).*\.o$$" | xargs rm -fv
> rm -fv $(NAME) *$(TNAME)
