
# Parser

Creates a linked list of statement trees

Parse left to right without precedence

## Operation

1. Initialize a list for the main statements
2. Parse next statement and repeat statement parsing, until tokeniser END

### Modes

#### Statement

Add values, ops and list into current tree

At end of tree traverse back to root node and return root

#### List

Add new tree to list of trees

### Command Resolution

### Type Resolution

### Op Resolution

## Dependency Graph

```
type -> command
command -> op
op -> init
init -> join
join -> next
next -> table
table -> parser
```
