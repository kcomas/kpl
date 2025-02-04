# KPL - Kevin's Programming Language

## Typed/Jitted Scripting Language written in C

## Supported Platforms and Dependencies

x64 Linux
GCC
GNU Make

## Tour

### Comments

Double slash until end of line

// my comment

### Values

Integer: 1, 234, -123
Float: 0.1, 3.14
String: "Hello World\n"

### Variables

UTF8 alpha numeric, first character cannot be numeric

#### : - Assign

a: 1
Π: 3.14
c: "Hello Word\n"

### Lists

All statements in lists are separated by ; or \n

#### {} Deferred

#### () Apply

#### [] Value

### SL - Symbols

\`key

### TYPE\`SYMBOL Tag

value\`key

### UN(...) Unions

Tagged Union

a\`a: 1 -> UN(I6\`a;\`a)
a\`b:2.2 -> UN(I6\`a;F6\`b;\`b)

#### Match ?

Get a floating value from a because the \`a is an int it must be changed

c: a?{1.0\`a;a\`b\`b}

### Numbers

#### Number Types

##### U6 - Unsigned 64bit integer

##### I6 - Signed 64bit integer

##### F6 - 64bit floating point

#### Checked Number Types

Operations on these types result in a union of value and error to check for underflow/overflow

##### U3 - Unsigned 8bit integer

##### I3 - Signed 8bit integer

##### U4 - Unsigned 16bit integer

##### I4 - Signed 16bit integer

##### U5 - Unsigned 32bit integer

##### I5 - Signed 32bit integer

##### F5 - 32bit floating point

### Functions
