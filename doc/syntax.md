# Syntax

## Encoding

Code is ASCII, strings are UTF-8

## Comments

```
// Comment Until End Of Line

/* Comment */
```

## Statements

Each statement is terminated with `;` or `\n`, these are used interchangeably

## Names

All user defined and built in names use a-zA-Z0-0_, 40 character limit

### Variables

Start with a lowercase letter

### Types

Start with an uppercase letter

#### Type Alias

### Symbols

Start with a ., character limit does not include .

## Unused Names

If a variable/type/symbol is not used it's name is prefixed with an ., for symbols ._

## No Keywords

See [Commands](./comamnds.md)

## Lists

### Define []

For creating types and getting names from types

```
Vec_i64 : Vector[I64] // Alias to type
```

### Apply ()

Function calls, array indexes, type creation

```
vec : Vector(1; 2; 3) // Vector[I64] with elements
```

### Lambda {}

Statement list

```
{ statement; statement
    statement
}
```

#### Lambda Arguments

For functions, matches ...

```
vec : Vector(1; 2; 3)
vec @ { I64.x; x\log } // print each item in a vector
vec @ { I64.x\log } // same as above
vec @ { .x\log } // same as above
```

## Operators

No Precedence

All operators can be called with () syntax

```
1 + 2 -> +(1; 2)
```
