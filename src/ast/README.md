
# Ast

Node placeholders, most of the AST information will be in the type

## Node Types

All nodes must have a type, only the topmost node with the ast string does not

### Atom

Type by itself

### List

Type with a list of ast nodes

### Op

Type with a left ast node and right ast node

## Root

The root of the ast is always a Lambda(List)

The root node has a parent node with the string the ast was built from and used for printing
