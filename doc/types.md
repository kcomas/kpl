
# Types

## Type Declaration Type[...]

## Type Definition Type(...)

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

#### Error

#### [Fd](./file_system.md)

#### Socket

### Fixed[Inner Type; Length]

#### [Array](./types/array.md)

### Tmp

### Vector[Inner Type]

#### [Vector](./types/vector.md)

#### [Set](./types/set.md)

#### [Result](./types/result.md)

#### [Option](./types/option.md)

#### [Closure](./functions.md)

#### [Thread](./threading.md)

### List[Type; Type; ...]

#### Op

#### [Map](./types/map.md)

#### [Overload](./functions.md)

### Table[Type; Type\`Symbol : Value; ...]

#### [Enum](./types/enum.md)

#### Range

#### [Struct](./types/struct.md)

#### [Union](./types/union.md)

#### [Fn](./functions.md)

#### [Base](./namespaces.md)

#### [Coroutine](./functions.md)

#### [Lambda](./functions.md)

### [Tag Type\`Symbol : Data](./types/tag.md)

#### Tag[Type]

#### Alias

A user defined type

```
Array_i64_len_3 : Array[I64; 3]
x :: Array_i64_len_3 ( 1; 2; 3 )
```

#### Var

#### Arg

## [Qualifiers](./types/qualifiers.md)
