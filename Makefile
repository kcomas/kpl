
.RECIPEPREFIX = >
CC = gcc
CFLAGS = -std=c17 -g -Wall -Wextra
SRC = ./src
TEST = ./test
SRCS = $(wildcard $(SRC)/*.c)
TSRCS = $(wildcard $(TEST)/*.c)
CCOBJ = $(CC) -o $@ $^
OBJS = $(patsubst %.c, %.o, $(SRCS))
NAME = kpl
TNAME = _test

all: $(NAME)

MOD_OBJS = $(SRC)/mod.o
mod$(TNAME): $(MOD_OBJS) $(TEST)/mod.o
> $(CCOBJ)

TKN_OBJS = $(SRC)/tkn.o $(MOD_OBJS)
tkn$(TNAME): $(TKN_OBJS) $(TEST)/tkn.o
> $(CCOBJ)

TBL_OBJS = $(SRC)/tbl.o
tbl$(TNAME): $(TBL_OBJS) $(TEST)/tbl.o
> $(CCOBJ)

AST_OBJS = $(SRC)/ast.o $(TKN_OBJS)
ast$(TNAME): $(AST_OBJS) $(TEST)/ast.o
> $(CCOBJ)

$(NAME):

%.o: %.c
> $(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
> rm -fv $(SRC)/*.o $(TEST)/*.o $(NAME) *$(TNAME)
