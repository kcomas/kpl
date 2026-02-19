
# Types

## Type Declaration Type[...]

## Type Definition Type(...)

## Classes

How a type is stored internally

### Symbol

A unique identifier, cannot be stored

### Scalar

#### Void

#### [Bool](./types/bool.md)

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

#### Error

#### [Buffer](./file_system.md)

#### [Fd](./file_system.md)

#### Socket

### Fixed[Inner Type; Length]

#### [Array](./types/array.md)

### Tmp

### Vector[Inner Type]

#### [Vector](./types/vector.md)

#### [Set](./types/set.md)

#### Queue

#### [Result](./types/result.md)

#### [Option](./types/option.md)

#### [Closure](./functions.md)

#### [Task](./threading.md)

### List[Type; Type; ...]

#### Op

#### [Command](./types/command.md)

#### [Tuple](./types/pair.md)

#### [Map](./types/map.md)

#### [Overload](./functions.md)

### Table[Type; Type\`Symbol : Value; ...]

#### [Enum](./types/enum.md)

#### [Struct](./types/struct.md)

#### [Union](./types/union.md)

#### [Fn](./functions.md)

#### [Coroutine](./functions.md)

#### [Lambda](./functions.md)

#### [Regex](./types/regex.md)

#### Range

### [Tag Type\`Symbol Data](./types/tag.md)

#### Tag[Type]

#### Tag

Unresolved Name, values are copied out

#### Value

#### Alias

A user defined type

```
Array_i64_len_3 : Array[I64; 3]
x :: Array_i64_len_3 ( 1; 2; 3 )
```

#### Var

#### Arg

### [Base](./namespaces.md)

## [Qualifiers](./types/qualifiers.md)
