
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

all: OO = -O3
all: $(NAME)

TT = test$(LTNAME)
LTEST_OBJS = $(LTEST)/t.o
$(TT): $(LTEST_OBJS) $(LTEST)/test.o
> $(CCOBJ)

DEF = def$(LTNAME)
LTT += $(DEF)
LDEF_OBJS = $(LSRC)/def.o
$(DEF): $(LDEF_OBJS) $(LTEST)/def.o $(LTEST_OBJS)
> $(CCOBJ)

MC = mc$(LTNAME)
LTT += $(MC)
LMC_OBJS = $(LSRC)/mc.o
$(MC): $(LMC_OBJS) $(LTEST)/mc.o $(LTEST_OBJS)
> $(CCOBJ)

X64 = x64$(LTNAME)
LTT += $(X64)
LX64_OBJS = $(LSRC)/x64.o
$(X64): $(LX64_OBJS) $(LTEST)/x64.o $(LTEST_OBJS)
> $(CCOBJ)

TE = te$(LTNAME)
LTT += $(TE)
LTE_OBJS = $(LSRC)/te.o
$(TE): $(LTE_OBJS) $(LTEST)/te.o $(LTEST_OBJS)
> $(CCOBJ)

VR = vr$(LTNAME)
LTT += $(VR)
LVR_OBS = $(LSRC)/vr.o
$(VR): $(LVR_OBS) $(LTEST)/vr.o $(LTEST_OBJS)
> $(CCOBJ)

LST = lst$(LTNAME)
LTT += $(LST)
LLST_OBJS = $(LSRC)/lst.o $(LTE_OBJS)
$(LST): $(LLST_OBJS) $(LTEST)/lst.o $(LTEST_OBJS)
> $(CCOBJ)

TBL = tbl$(LTNAME)
LTT += $(TBL)
LTBL_OBJS = $(LSRC)/tbl.o $(LLST_OBJS)
$(TBL): $(LTBL_OBJS) $(LTEST)/tbl.o $(LTEST_OBJS)
> $(CCOBJ)

TKN = tkn$(LTNAME)
LTT += $(TKN)
LTKN_OBJS = $(SRC)/tkn.o $(LSRC)/tkn.o $(LTBL_OBJS) $(LMC_OBJS) $(LDEF_OBJS)
$(TKN): $(LTKN_OBJS) $(LTEST)/tkn.o $(LTEST_OBJS)
> $(CCOBJ)

PSR = psr$(LTNAME)
LTT += $(PSR)
LPSR_OBJS = $(SRC)/psr.o $(LSRC)/psr.o $(LVR_OBS) $(LTKN_OBJS) $(LVR_OBS)
$(PSR): $(LPSR_OBJS) $(LTEST)/psr.o $(LTEST_OBJS)
> $(CCOBJ)

AS = as$(LTNAME)
LTT += $(AS)
LAS_OBJS = $(LSRC)/as.o $(LTBL_OBJS)
LASX64_OBJS = $(LSRC)/as_x64.o $(LX64_OBJS)
$(AS): $(LAS_OBJS) $(LTEST)/as.o $(LTEST)/as_t.o $(LASX64_OBJS) $(LTEST_OBJS)
> $(CCOBJ)

GEN = gen$(LTNAME)
LTT += $(GEN)
GEN_OBJS = $(LSRC)/gen.o $(LAS_OBJS)
GENX64_OBJS = $(GEN_OBJS) $(LSRC)/gen_x64.o $(LX64_OBJS) $(LASX64_OBJS) $(LVR_OBS) $(patsubst %.c,%.o,$(wildcard $(LSRC)/gen_x64/*.c))
$(GEN): $(GENX64_OBJS) $(LTEST)/gen.o $(LTEST)/gen_t.o $(LTEST)/as_t.o $(LTEST_OBJS)
> $(CCOBJ)

AST = ast$(LTNAME)
LTT += $(AST)
AST_OBJS = $(SRC)/ast.o $(LSRC)/ast.o $(LPSR_OBJS)
$(AST): $(AST_OBJS) $(LTEST)/ast.o $(LTEST_OBJS)
> $(CCOBJ)

lib_tests: $(LTT)

clean:
> find -type f -regex "^\($(SRC)\|$(TEST)\).*\.o$$" | xargs rm -fv
> rm -fv $(NAME) *$(TNAME)
