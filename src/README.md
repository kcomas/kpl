
# Implementation

## Directory Structure

Each directory has a directory_name.h file as a single header for all of it's parts

### Namespace

### [Type](./type/READEME.md)

### Ast

### Parser

### Fold

### Check

### Eval

### Repl

### Ir

### Gen

### [X64](./x64/README.md)

### [Run](./run/README.md)

### [Lib](./lib/README.md)

### [Core](./core/README.md)

### Repl

## main.c

Application entry point

## Dependency Graph

```
Lib -> X64
Lib -> Run
Lib -> Tkn
Run -> Type
Type -> Namespace
Type -> X64
Namespace -> Ast
Ast X64 -> Ir
Ast Tkn -> Parser
Parser -> Fold
Fold -> Check
Check -> Eval
Eval -> Repl
Eval Ir -> Gen
Gen -> Opt
Opt -> Core
Core -> Repl
Repl -> main.c
```
