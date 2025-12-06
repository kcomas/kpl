
# Syntax

## Comments

Removed in parsing

```
// single line until newline

/*
    multi line
*/
```

## Naming

All types, variables symbols can be named with a-zA-Z0-9_ with a 40 character limit

Variables start with a lowercase letter

Types start with an uppercase letter

Symbols start with \`

_ denotes the variable is ignored

No keywords, use commands \name

## Operators

No Operator Precedence

Read from right to left

All operators can be called as a function

Statement List can be used for precedence

Reads until end of statement

`7 = 1 + 2 * 3`

`9 = 3 * 2 + 1`

`7 = *(3; 2) + 1`

`7 = { 3 * 2 } + 1`


```
// invalid
1 +
    2
// valid
1 + {
    2
}
```

## Statements

; and \n are used interchangeably to end a statement

## Lists

```
Array { 1; 2; 3 }
Array {
    1
    2
    3
}
Array { 1
    2
3 }
```

#### {} List of Statements

By default statements from left to right "returning" the value from the last statement

Types and operators change statement behavior

#### () List of Arguments

Apply list of statements to a target

#### [] List of Types

For type definitions

## Grammar

Precedence is from left to right

Statement <- Op -> Statement

Statement <- List, Symbol, Command

Statement = Var, Value, Type, List, Symbol, Command, Op
