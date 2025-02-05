# KPL - Kevin's Programming Language

## Typed/Jitted Scripting Language written in C

## Supported Platforms and Dependencies

X86-64 Linux

GCC

GNU Make

## Build TODO

## Usage TODO

## Tour

### Comments

Double slash until end of line

```
// my comment
```

### Values

Integer: 1, 234, -123

Float: 0.1, 3.14

String: "Hello World\n"

### Variables

UTF8 alpha numeric, first character cannot be numeric

#### : Assign

```
a: 1
Π: 3.14
c: "Hello Word\n"
```

### Lists

All statements in lists are separated by ; or \n

#### {} Deferred

A list of statements of which the context it is used determines how it's evaluated

#### () Apply

#### [] Value

### Symbols

##### SL

```
`key
```

### Tag

##### TYPE\`SYMBOL

```
value`key
```

### Unions

##### UN(TYPE`key;TAG;....)

Tagged Union

```
a`a: 1 -> UN(I6`a;`a)
a`b:2.2 -> UN(I6`a;F6`b;`b)
```

#### Match ?

Get a floating value from var a because the a\`a tag is an int it must be changed

```
c: a?{1.0`a;a`b`b}
```

### Numbers

#### Number Ops

```
+ Add
- Sub
* Mul
/ Div
% Rem
```

#### Number Types

```
a: 1 -> I6
Π: 3.14 -> F6
```

##### U6 - Unsigned 64bit integer

##### I6 - Signed 64bit integer

##### F6 - 64bit floating point

### Casting

#### TO$FROM

Convert data from one type to another

###### Int To Float Example

```
a: F6$1 -> a = 1.0
```

#### Checked Number Types

Operations on these types result in a union of value and error to check for underflow/overflow

```
a: I3$128 + I3$1 -> UN(I3`v;TODO`e)
v: a?{a`v`v;0`e} -> v = 0
```

##### U3 - Unsigned 8bit integer

##### I3 - Signed 8bit integer

##### U4 - Unsigned 16bit integer

##### I4 - Signed 16bit integer

##### U5 - Unsigned 32bit integer

##### I5 - Signed 32bit integer

##### F5 - 32bit floating point

### Functions

Create a function by casting a deferred list to a function

##### FN(TYPE\`VAR;...RETURN TYPE)${BODY}

```

f: FN(I6`x;I6`y;I6)${x+y}
f(1;2) -> 3

```

Functions are curried by default

```
f(1) -> FN(I6`b)${1+b} -> f(2) -> 3

```
