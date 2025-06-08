
CC = gcc
OO = -g -Og
FFLAGS = -fno-omit-frame-pointer -D_FORTIFY_SOURCE=3 -D_GLIBCXX_ASSERTIONS -ftrivial-auto-var-init=pattern -fPIE -fstack-protector-all -fstack-clash-protection -fcf-protection=full
MFLAGS = -mincoming-stack-boundary=3
WFLAGS = -Wall -Wextra -Wstack-protector
CFLAGS = -std=gnu99 $(FLAGS) $(OO) $(FFLAGS) $(MFLAGS) $(WFLAGS)
SRC = ./src
LSRC = $(SRC)/lib
TEST = ./test
LFLAGS = -pie -Wl,-z,relro,-z,now
CCOBJ = $(CC) -o $@ $^ $(LFLAGS)
NAME = kpl
TNAME = _test
TESTS = tests
.RECIPEPREFIX = >
.PHONY: show_$(TESTS) clean

all: $(NAME)

TEST_OBJS = $(TEST)/t.o

DEF = def$(TNAME)
LDEF_OBJS = $(LSRC)/def.o
OBJS += $(LDEF_OBJS)
$(DEF): $(LDEF_OBJS) $(TEST)/def.o $(TEST_OBJS)
> $(CCOBJ)

ERR = err$(TNAME)
LERR_OBJS = $(LSRC)/err.o
OBJS += $(LERR_OBJS)
$(ERR): $(LERR_OBJS) $(TEST)/err.o $(TEST_OBJS)
> $(CCOBJ)

MC = mc$(TNAME)
LMC_OBJS = $(LSRC)/mc.o
OBJS += $(LMC_OBJS)
$(MC): $(LMC_OBJS) $(TEST)/mc.o $(TEST_OBJS)
> $(CCOBJ)

X64 = x64$(TNAME)
LX64_OBJS = $(LSRC)/x64.o
OBJS += $(LX64_OBJS)
$(X64): $(LX64_OBJS) $(TEST)/x64.o $(TEST_OBJS)
> $(CCOBJ)

TE = te$(TNAME)
LTE_OBJS = $(LSRC)/te.o
OBJS += $(LTE_OBJS)
$(TE): $(LTE_OBJS) $(TEST)/te.o $(TEST_OBJS)
> $(CCOBJ)

VR = vr$(TNAME)
LVR_OBS = $(LSRC)/vr.o
OBJS += $(LVR_OBS)
$(VR): $(LVR_OBS) $(TEST)/vr.o $(TEST_OBJS)
> $(CCOBJ)

LST = lst$(TNAME)
LLST_OBJS = $(LSRC)/lst.o $(LTE_OBJS)
OBJS += $(LLST_OBJS)
$(LST): $(LLST_OBJS) $(TEST)/lst.o $(TEST_OBJS)
> $(CCOBJ)

TBL = tbl$(TNAME)
LTBL_OBJS = $(LSRC)/tbl.o $(LLST_OBJS) $(LMC_OBJS)
OBJS += $(LTBL_OBJS)
$(TBL): $(LTBL_OBJS) $(TEST)/tbl.o $(TEST_OBJS)
> $(CCOBJ)

TKN = tkn$(TNAME)
TKN_OBJS = $(SRC)/tkn.o $(LSRC)/tkn.o $(LERR_OBJS) $(LTBL_OBJS) $(LMC_OBJS) $(LDEF_OBJS)
OBJS += $(TKN_OBJS)
$(TKN): $(TKN_OBJS) $(TEST)/tkn.o $(TEST_OBJS)
> $(CCOBJ)

PSR = psr$(TNAME)
PSR_OBJS = $(SRC)/psr.o $(LSRC)/psr.o $(LVR_OBS) $(TKN_OBJS) $(LVR_OBS)
OBJS += $(PSR_OBJS)
$(PSR): $(PSR_OBJS) $(TEST)/psr.o $(TEST)/psr_t.o $(TEST_OBJS)
> $(CCOBJ)

AS = as$(TNAME)
LAS_OBJS = $(LSRC)/as.o $(LERR_OBJS) $(LTBL_OBJS) $(LDEF_OBJS)
LASX64_OBJS = $(LSRC)/as_x64.o $(LX64_OBJS) $(patsubst %.c,%.o,$(wildcard $(LSRC)/as_x64/*.c))
OBJS += $(LAS_OBJS) $(LASX64_OBJS)
$(AS): $(LAS_OBJS) $(TEST)/as.o $(TEST)/as_t.o $(LASX64_OBJS) $(TEST_OBJS)
> $(CCOBJ)

GEN = gen$(TNAME)
GEN_OBJS = $(LSRC)/gen.o $(LAS_OBJS)
GENX64_OBJS = $(GEN_OBJS) $(LSRC)/gen_x64.o $(LX64_OBJS) $(LASX64_OBJS) $(LVR_OBS) $(patsubst %.c,%.o,$(wildcard $(LSRC)/gen_x64/*.c))
OBJS += $(GEN_OBJS) $(GENX64_OBJS)
$(GEN): $(GENX64_OBJS) $(TEST)/gen.o $(TEST)/gen_t.o $(TEST)/as_t.o $(TEST_OBJS)
> $(CCOBJ)

TYPE = type$(TNAME)
TYPE_OBJS = $(LSRC)/type.o $(LTBL_OBJS)
OBJS += $(TYPE_OBJS)
$(TYPE): $(TYPE_OBJS) $(TEST)/type.o $(TEST_OBJS)
> $(CCOBJ)

AST = ast$(TNAME)
AST_OBJS = $(SRC)/ast.o $(LSRC)/ast.o $(TYPE_OBJS) $(PSR_OBJS)
OBJS += $(AST_OBJS)
$(AST): $(AST_OBJS) $(TEST)/ast.o $(TEST)/psr_t.o $(TEST)/ast_t.o $(TEST_OBJS)
> $(CCOBJ)

FLD = fld$(TNAME)
FLD_OBJS = $(SRC)/fld.o $(LSRC)/fld.o $(AST_OBJS)
OBJS += $(FLD_OBJS)
$(FLD): $(FLD_OBJS) $(TEST)/fld.o $(TEST)/fld_t.o $(TEST)/ast_t.o $(TEST)/psr_t.o $(TEST_OBJS)
> $(CCOBJ)

CHK = chk$(TNAME)
CHK_OBJS = $(SRC)/chk.o $(LSRC)/chk.o $(FLD_OBJS) $(patsubst %.c,%.o,$(wildcard $(SRC)/chk/*.c))
OBJS += $(CHK_OBJS)
$(CHK): $(CHK_OBJS) $(TEST)/chk.o $(TEST)/chk_t.o $(TEST)/fld_t.o $(TEST)/ast_t.o $(TEST)/psr_t.o $(TEST_OBJS)
> $(CCOBJ)

OPT = opt$(TNAME)
OPT_OBJS = $(SRC)/opt.o $(CHK_OBJS)
OBJS += $(OPT_OBJS)
$(OPT): $(OPT_OBJS) $(TEST)/opt.o $(TEST)/opt_t.o $(TEST)/chk_t.o $(TEST)/fld_t.o $(TEST)/ast_t.o $(TEST)/psr_t.o $(TEST_OBJS)
> $(CCOBJ)

ATG = atg$(TNAME)
ATG_OBJS = $(LSRC)/atg.o $(OPT_OBJS) $(GEN_OBJS)
ATGX64_OBJS = $(SRC)/atg_x64.o $(GENX64_OBJS) $(patsubst %.c,%.o,$(wildcard $(SRC)/atg_x64/*.c))
OBJS += $(ATG_OBJS) $(ATGX64_OBJS)
$(ATG): $(sort $(ATG_OBJS) $(ATGX64_OBJS) $(TEST)/atg.o $(TEST)/gen_t.o $(TEST)/as_t.o $(TEST_OBJS) $(TEST)/opt_t.o $(TEST)/chk_t.o $(TEST)/fld_t.o $(TEST)/ast_t.o $(TEST)/psr_t.o $(TEST_OBJS))
> $(CCOBJ)

OBJS := $(sort $(OBJS))

FSAN = -fsanitize=address,leak,undefined
#$(TESTS): OO = -O2
$(TESTS): FLAGS += -DNTO
$(TESTS): FFLAGS += $(FSAN)
$(TESTS): LFLAGS += $(FSAN)
$(TESTS): WFLAGS += -Werror
$(TESTS): $(OBJS) $(patsubst %.c,%.o,$(wildcard $(TEST)/*.c))
> $(CCOBJ)

show_$(TESTS):
> find ./test -type f -name "*.c" | grep -v -E "(_t|\/t\.c)" | cut -d "/" -f3 | sed 's/\.c/$(TNAME)/'

$(NAME): OO = -O2
$(NAME): WFLAGS += -Werror
$(NAME): $(SRC)/main.o $(SRC)/z.o $(OBJS)
> $(CCOBJ)

clean:
> find -type f -regex "^\($(SRC)\|$(TEST)\).*\.o$$" | xargs rm -fv
> rm -fv $(NAME) *$(TNAME) $(TESTS)
