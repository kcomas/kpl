
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
CCOBJ = $(CC) -o $@ $^
NAME = kpl
TNAME = _test
LTNAME = _lib$(TNAME)

all: OO = -O3
all: $(NAME)

TT = test$(LTNAME)
TEST_OBJS = $(TEST)/t.o
$(TT): $(TEST_OBJS) $(TEST)/test.o
> $(CCOBJ)

DEF = def$(LTNAME)
LTT += $(DEF)
LDEF_OBJS = $(LSRC)/def.o
$(DEF): $(LDEF_OBJS) $(TEST)/def.o $(TEST_OBJS)
> $(CCOBJ)

MC = mc$(LTNAME)
LTT += $(MC)
LMC_OBJS = $(LSRC)/mc.o
$(MC): $(LMC_OBJS) $(TEST)/mc.o $(TEST_OBJS)
> $(CCOBJ)

X64 = x64$(LTNAME)
LTT += $(X64)
LX64_OBJS = $(LSRC)/x64.o
$(X64): $(LX64_OBJS) $(TEST)/x64.o $(TEST_OBJS)
> $(CCOBJ)

TE = te$(LTNAME)
LTT += $(TE)
LTE_OBJS = $(LSRC)/te.o
$(TE): $(LTE_OBJS) $(TEST)/te.o $(TEST_OBJS)
> $(CCOBJ)

VR = vr$(LTNAME)
LTT += $(VR)
LVR_OBS = $(LSRC)/vr.o
$(VR): $(LVR_OBS) $(TEST)/vr.o $(TEST_OBJS)
> $(CCOBJ)

LST = lst$(LTNAME)
LTT += $(LST)
LLST_OBJS = $(LSRC)/lst.o $(LTE_OBJS)
$(LST): $(LLST_OBJS) $(TEST)/lst.o $(TEST_OBJS)
> $(CCOBJ)

TBL = tbl$(LTNAME)
LTT += $(TBL)
LTBL_OBJS = $(LSRC)/tbl.o $(LLST_OBJS)
$(TBL): $(LTBL_OBJS) $(TEST)/tbl.o $(TEST_OBJS)
> $(CCOBJ)

TKN = tkn$(LTNAME)
LTT += $(TKN)
TKN_OBJS = $(SRC)/tkn.o $(LSRC)/tkn.o $(LTBL_OBJS) $(LMC_OBJS) $(LDEF_OBJS)
$(TKN): $(TKN_OBJS) $(TEST)/tkn.o $(TEST_OBJS)
> $(CCOBJ)

PSR = psr$(LTNAME)
LTT += $(PSR)
PSR_OBJS = $(SRC)/psr.o $(LSRC)/psr.o $(LVR_OBS) $(TKN_OBJS) $(LVR_OBS)
$(PSR): $(PSR_OBJS) $(TEST)/psr.o $(TEST_OBJS)
> $(CCOBJ)

AS = as$(LTNAME)
LTT += $(AS)
LAS_OBJS = $(LSRC)/as.o $(LTBL_OBJS)
LASX64_OBJS = $(LSRC)/as_x64.o $(LX64_OBJS)
$(AS): $(LAS_OBJS) $(TEST)/as.o $(TEST)/as_t.o $(LASX64_OBJS) $(TEST_OBJS)
> $(CCOBJ)

GEN = gen$(LTNAME)
LTT += $(GEN)
GEN_OBJS = $(LSRC)/gen.o $(LAS_OBJS)
GENX64_OBJS = $(GEN_OBJS) $(LSRC)/gen_x64.o $(LX64_OBJS) $(LASX64_OBJS) $(LVR_OBS) $(patsubst %.c,%.o,$(wildcard $(LSRC)/gen_x64/*.c))
$(GEN): $(GENX64_OBJS) $(TEST)/gen.o $(TEST)/gen_t.o $(TEST)/as_t.o $(TEST_OBJS)
> $(CCOBJ)

AST = ast$(LTNAME)
LTT += $(AST)
AST_OBJS = $(SRC)/ast.o $(LSRC)/ast.o $(PSR_OBJS)
$(AST): $(AST_OBJS) $(TEST)/ast.o $(TEST_OBJS)
> $(CCOBJ)

tests: $(LTT)

clean:
> find -type f -regex "^\($(SRC)\|$(TEST)\).*\.o$$" | xargs rm -fv
> rm -fv $(NAME) *$(TNAME)
