
# Types

## Classes

How a type is stored internally

### Scalar

### Array[Inner Type; Length]

### Vector[Inner Type]

### Op[Return Type; Right Type]

### Op[Return Type; Left Type; Right Type]

### Struct {Type\`Symbol...}

### Union[\`Active Symbol] { Type\`Symbol...}

### Hash[Key Type; Value Type]

### Fn[Return Type] {Type\`Symbol...}

### Lambda[Return Type] {Type\`Arg Symbol...} {Type\`Scope Symbol...}

## Qualifiers

Type modifications

### Const[Type]

### Ref[Type]

### Namespace[Type]

## Tags

A type associated with a symbol

## Built In Types

## Aliases

A user define type

```
Array_i64_len_3::Array[I64; 3]
x :: Array_i64_len_3 { 1; 2; 3 }
```
