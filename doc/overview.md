# Overview

## General

* Statically typed

* "Staitc Jit" generates native code at runtime, but all functions types can be statically determined

* Async first, any function can be called async or wait for an async function

* APL style code but with keywords instead of symbols, no operator precedence

## Syntax

### Comments

```
// Comment

//* Comment */
```

### Variables

Start with a lowercase letter, allows [a-zA-Z0-9_], 40 character limit

Start a variable _ to designate the variable as unused

### Types

Start with an uppercase letter, allows [A-Za-z0-9_], 40 character limit

### Symbols

Start with `.` allows [a-zA-Z0-9_], 40 character limit not including `.`

### Statements

Expressions, 1 + 2 - 3 etc

### Ending Statements and Separating List Items

`\n` and `;` are used interchangeably to end statements and separate items in a list

### Operators

No precedence

Can be called like functions

```
1 + 2
+(1; 2)
```

### Keywords

Start with \`, allows [a-zA-Z0-9_], 40 character limit not including \`

Used like traditional operators or functions

```
`log "Hello Word" // String : "Hello World"
`log("Hello World") // String : "Hello World"
```

### Lists

"List" of statements, list type determines how the statements are used

#### Lambda {}

Execute statements from left to right, top to bottom

#### Definition []

Create types and load symbols into the current scope

#### Apply ()

Apply statements to a target
