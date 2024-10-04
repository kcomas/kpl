
.PHONY: clean
.RECIPEPREFIX = >
CC = gcc
DFLAGS = -DALD=1
CFLAGS = $(DFLAGS) -g -Og -fstack-protector-all -Wall -Wextra -Wstack-protector # TODO specify std
SRC = ./src
TEST = ./test
SRCS = $(wildcard $(SRC)/*.c)
TSRCS = $(wildcard $(TEST)/*.c)
CCOBJ = $(CC) -o $@ $^
OBJS = $(patsubst %.c, %.o, $(SRCS))
NAME = kpl
TNAME = _test

all: $(NAME)

AL_OBJS = $(SRC)/al.o $(SRC)/kpl.o
al$(TNAME): $(AL_OBJS) $(TEST)/al.o
> $(CCOBJ)

ER_OBJS = $(SRC)/er.o $(AL_OBJS)

MOD_OBJS = $(SRC)/mod.o $(SRC)/var.o $(ER_OBJS)
mod$(TNAME): $(MOD_OBJS) $(TEST)/mod.o
> $(CCOBJ)

TKN_OBJS = $(SRC)/tkn.o $(MOD_OBJS)
tkn$(TNAME): $(TKN_OBJS) $(TEST)/tkn.o
> $(CCOBJ)

TBL_OBJS = $(SRC)/tbl.o $(AL_OBJS)
tbl$(TNAME): $(TBL_OBJS) $(TEST)/tbl.o
> $(CCOBJ)

AST_OBJS = $(SRC)/ast.o $(TKN_OBJS) $(TBL_OBJS)
ast$(TNAME): $(AST_OBJS) $(TEST)/ast.o
> $(CCOBJ)

TYPE_OBJS = $(SRC)/type.o $(AST_OBJS)
type$(TNAME): $(TYPE_OBJS) $(TEST)/type.o
> $(CCOBJ)

CODE_OBJS = $(SRC)/code.o $(TYPE_OBJS)
code$(TNAME): $(CODE_OBJS) $(TEST)/code.o
> $(CCOBJ)

JIT_OBJS = $(SRC)/jit.o $(CODE_OBJS)
jit$(TNAME): $(JIT_OBJS) $(TEST)/jit.o
> $(CCOBJ)

$(NAME): $(SRC)/main.o $(JIT_OBJS)
> $(CCOBJ)

%.o: %.c %.h
> $(CC) $(CFLAGS) -c $< -o $@

clean:
> rm -fv $(SRC)/*.o $(TEST)/*.o $(NAME) *$(TNAME)
