
# Types

## Classes

How a type is stored internally

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

#### Mask

#### Char

#### String

#### Regex

#### Root

#### Error

#### Fd

#### Socket

### Array[Inner Type; Length]

#### Array

### Vector[Inner Type]

#### Vector

#### Set

#### Result

#### Closure

#### Thread

#### Coroutine

### Monadic[Return Type; Right Type]

### Dyadic[Return Type; Left Type; Right Type]

### Struct {Type\`Symbol...}

#### Struct

#### Table

### Union[\`Active Symbol] { Type\`Symbol...}

#### Union

### Hash[Key Type; Value Type]

#### Hash

### Fn[Return Type] {Type\`Symbol...}

#### Fn

### Lambda[Return Type] {Type\`Arg Symbol...} {Type\`Scope Symbol...}

#### Lambda

### C[Type]

## Tags

A type associated with a symbol

## Qualifiers

Type modifications, only one can be applied

### Const[Type]

### Ref[Type]

### Namespace[Type]

### Shared[Type]

### Mutex[Type]

## Aliases

A user defined type

```
Array_i64_len_3 :: Array[I64; 3]
x :: Array_i64_len_3 { 1; 2; 3 }
```
