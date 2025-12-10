
# Implementation

## Directory Structure

Each directory has a directory_name.h file as a single header for all of it's parts

### Namespace

### Type

### Ast

### Import

### Parser

### Check

### Eval

### Ir

### Gen

### [X64](./x64/README.md)

### [Run](./run/README.md)

### [Lib](./lib/README.md)

### [Core](./core/README.md)

## main.c

Application entry point

## Dependency Graph

Lib -> X64
Lib -> Run
Lib -> Import
Lib Run -> Namespace
Namespace -> Type
Type X64 -> Ir
Type -> Ast
Ast Ir -> Gen
Gen -> Eval
Eval -> Check
Check -> Parser
Import Parser -> Core
Core -> main.c
