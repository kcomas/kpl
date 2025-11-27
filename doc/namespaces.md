
# Namespaces

How code/data is shared during runtime

`.name. ... name`

## Exporting

```
"name" . var

{ "name" ; "name"; ... } . name

var\export // "var_name" . var
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

## Built In

### sys

#### args

```
.sys.args :: Namespace[Const[Arary[String] { determined at runtime }]]

argv0 .sys.args(0) // Result[Const[String]]
```

### io

#### Console

```
.io.Console :: Enum[U32] { `stdin { 0 }; `stdout { 1 }; `stderr { 2 } }

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
