
# Namespaces

How code/data is shared during runtime

`.name. ... name`

## Exporting

```
var\export
```

## Importing

File names are relative to the importing file

Imports are done once at file load, importing in a function does not import again

### Base

A key to access an exported namespace

```
var : "./path/to/file/"\import
var.fn(...)
```

### Destructuring

Names can be moved out of a base into the scope

```
[ fn ] : "./file"\import
fn(...)
```

## Built In

### sys

#### args

```
.sys.args : Namespace[Const[Arary[String;determined at runtime]]]
argv0 : .sys.args`0 // Const[String]
```

#### sleep

If interrupted by signal returns error

```
.sys.sleep : Namespace[Overload {
    Fn[Result<Void>] { U64`seconds }
    Fn[Result<Void>] { U64`seconds; U64`nano_seconds }
}]]
```

#### run

### io

#### Console

```
.io.Console : Enum[I32] { `stdin $ 0; `stdout $ 1; `stderr $ 2 }

```

#### stdin

```
.io.stdin : FD $ 0
```

#### stdout

```
.io.stdout : FD $ 1
```

#### stderr

```
.io.stderr : FD $ 2
```

#### colors.codes

Color codes for terminal printing

#### colors.strings

Color strings for terminal printing

### math

#### ceil

```
.math.ceil : Namespace[Overload {
    Fn[I32] { F32`number }
    Fn[I64] { F64`number }
}]
```

#### floor

```
.math.floor : Namespace[Overload {
    Fn[I32] { F32`number }
    Fn[I64] { F64`number }
}]
```

#### log

Natural logarithm

```
.math.log : Namespace[Overload {
    Fn[Result[F32]] { F32`number }
    Fn[Result[F64]] { F64`number }
}]
```

### regex

#### Flags

```
.regex.Flags : Enum[U8] { `invalid : 0; `d : 1 << 0; `g : 1 << 1; `i : 1 << 2; `m : 1 << 3; `s : 1 << 4 }
```

### thread

#### Status

```
.thread.Status : Enum[U8] { `invalid : 0; `running : 1 << 0; `done : 1 << 1; `error : 1 << 2 }
```
