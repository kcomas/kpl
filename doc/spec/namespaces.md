
# Namespaces

How code/data is shared during runtime

`.name. ... name`

## Exporting

```
var\export
```

## Importing

File names are relative to the importing file

Importing can only be done at the top scope

### Base

A key to access an exported namespace

```
var :: "./path/to/file/"\import
var.fn(...)
```

### Destructuring

Names can be moved out of a base into the scope

```
{ fn } :: "./file"\import
fn(...)
```

## Built In

### sys

#### args

```
.sys.args :: Namespace[Const[Arary[String;determined at runtime]]]
argv0 :: .sys.args`0 // Const[String]
```

### io

#### Console

```
.io.Console :: Enum[I32] { `stdin $ 0; `stdout $ 1; `stderr $ 2 }

```

#### print

```
.io.print :: Namespace[Overload {
    C[Fn[Void] { String`string }]
    C[Fn[Void] { .io.Console`to; String`string }]
}]
```

### math

#### ceil

```
.math.ceil :: Namespace[Overload {
    C[Fn[I32] { F32`number }]
    C[Fn[I64] { F64`number }]
}]
```

#### floor

```
.math.floor :: Namespace[Overload {
    C[Fn[I32] { F32`number }]
    C[Fn[I64] { F64`number }]
}]
```

#### log

Natural logarithm

```
.math.log :: Namespace[Overload {
    C[Fn[Result[F32]] { F32`number }]
    C[Fn[Result[F64]] { F64`number }]
}]
```

### regex

#### Flags

```
.regex.Flags : Enum[U8] { `invalid : 0; `d : 1 << 0; `g : 1 << 1; `i : 1 << 2; `m : 1 << 3; `s : 1 << 4 }
```
