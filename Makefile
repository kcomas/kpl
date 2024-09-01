
.PHONY: clean
.RECIPEPREFIX = >
CC = gcc
CFLAGS = -g -Og -Wall -Wextra # TODO specify std
SRC = ./src
TEST = ./test
SRCS = $(wildcard $(SRC)/*.c)
TSRCS = $(wildcard $(TEST)/*.c)
CCOBJ = $(CC) -o $@ $^
OBJS = $(patsubst %.c, %.o, $(SRCS))
NAME = kpl
TNAME = _test

all: $(NAME)

MOD_OBJS = $(SRC)/mod.o $(SRC)/var.o $(SRC)/kpl.o
mod$(TNAME): $(MOD_OBJS) $(TEST)/mod.o
> $(CCOBJ)

TKN_OBJS = $(SRC)/tkn.o $(MOD_OBJS)
tkn$(TNAME): $(TKN_OBJS) $(TEST)/tkn.o
> $(CCOBJ)

TBL_OBJS = $(SRC)/tbl.o
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

$(NAME):

%.o: %.c
> $(CC) $(CFLAGS) -c $< -o $@

clean:
> rm -fv $(SRC)/*.o $(TEST)/*.o $(NAME) *$(TNAME)
