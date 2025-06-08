
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

$(NAME):

TKN_OBJS = $(SRC)/tkn.o
tkn$(TNAME): $(TKN_OBJS) $(TEST)/tkn.o
> $(CCOBJ)

MOD_OBJS = $(SRC)/mod.o
mod$(TNAME): $(MOD_OBJS) $(TEST)/mod.o
> $(CCOBJ)

%.o: %.c
> $(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
> rm -f $(SRC)/*.o $(TEST)/*.o $(NAME) *$(TNAME)
