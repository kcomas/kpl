
# Types

## Classes

How a type is stored internally

### Symbol

A unique identifier, cannot be stored

### Scalar

#### Void

#### Bool

#### U8

#### U16

#### U32

#### U64

#### I8

#### I16

#### I32

#### I64

#### F32

#### F64

#### [Char](./types/string.md)

#### [String](./types/string.md)

#### [Regex](./types/regex.md)

#### Base

See namespaces

#### Error

#### Fd

#### Socket

### Array[Inner Type; Length]

#### Array

### Vector[Inner Type]

#### Vector

#### Set

#### [Result](./types/result.md)

#### [Option](./types/option.md)

#### Closure

#### Thread

### Op[Return Type; Left Type; Right Type]

### Enum[Type] { \`Symbol : Value }

#### [Enum](./types/enum.md)

### Struct {Type\`Symbol...}

#### Struct

#### Table

#### Union

### Hash[Key Type; Value Type]

#### Hash

### Fn[Return Type] {Type\`Symbol...}

#### Fn

#### Coroutine

### Lambda[Return Type] {Type\`Arg Symbol...} {Type\`Scope Symbol...}

#### Lambda

### Overload { Fn1; Fn2; ...}

#### Overload

### C[Type]

## [Tag](./types/tag.md)

## [Qualifiers](./types/qualifiers.md)

## Aliases

A user defined type

```
Array_i64_len_3 :: Array[I64; 3]
x :: Array_i64_len_3 { 1; 2; 3 }
```
