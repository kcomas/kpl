
# Implementation

## Directory Structure

Each directory has a directory_name.h file as a single header for all of it's parts

### Core

### Namespace

### Type

### Ast

### Parser

### Fold

### Check

### Eval

### Ir

### Gen

### X64

### [Run](./run/README.md)

### [Lib](./lib/README.md)

## main.c

Application entry point

## Dependency Graph

Lib -> X64
Lib -> Run
Lib Run -> Namespace
Namespace -> Type
Type X64 -> Ir
Type -> Ast
Ast -> Parser
Ast -> Fold
Ast -> Check
Ast -> Eval
Ast Ir -> Gen
Parser Fold Check Eval Gen -> Core
Core -> main.c
