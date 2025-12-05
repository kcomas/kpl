
# Types

## Classes

How a type is stored internally

### Symbol

A unique identifier, cannot be stored

### Scalar

#### Void

#### Bool

true, false, TRUE, FALSE

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

#### [Fd](./file_system.md)

#### Socket

### Array[Inner Type; Length]

#### [Array](./types/array.md)

### Vector[Inner Type]

#### [Vector](./types/vector.md)

#### [Set](./types/set.md)

#### [Result](./types/result.md)

#### [Option](./types/option.md)

#### [Closure](./functions.md)

#### [Thread](./threading.md)

### Op[Return Type; Left Type; Right Type]

### Enum[Type] { \`Symbol : Value }

#### [Enum](./types/enum.md)

### Struct {Type\`Symbol...}

#### [Struct](./types/struct.md)

#### Table

#### [Union](./types/union.md)

### Hash[Key Type; Value Type]

#### [Hash](./types/hash.md)

### Fn[Return Type] {Type\`Symbol...}

#### [Fn](./functions.md)

#### [Coroutine](./functions.md)

### Lambda[Return Type] {Type\`Arg Symbol...} {Type\`Scope Symbol...}

#### [Lambda](./functions.md)

### Overload { Fn1; Fn2; ...}

#### [Overload](./functions.md)

### C[Type]

## [Tag](./types/tag.md)

## [Qualifiers](./types/qualifiers.md)

## Aliases

A user defined type

```
Array_i64_len_3 :: Array[I64; 3]
x :: Array_i64_len_3 { 1; 2; 3 }
```
