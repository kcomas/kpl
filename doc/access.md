
# Access

## \` static accessing

Checked before code execution

## () dynamic accessing

Checked during code execution

\`index can be used for dynamic accessing but () cannot be used for static accessing

## Array

### Static access

```
string_array : Array { "this"; "is"; "an"; "array" } // Array[String]
var : string_array`1 // Ref[String]
```

### Dynamic access

```
string_array : Array { "this"; "is"; "an"; "array" } // Array[String]
n : 1
var : "Bad Index" ^ string_array(n) // Ref[String]
```

## Struct

### Static access only

```
struct : Struct { I64`a { 1 }; U64`b { 2 } }
struct`a // I64
```

## Vector / Hash

### Dynamic access only

```
num_vec : Vector[I64] { 1; 2; 3 }
var : "Bad Index" ^ num_vec`0 // I64
```

```
num_vec : Vector[I64] { 1; 2; 3 }
n : 1
var : "bad Index" ^ num_vec(n) // I64
```

## Dynamic Matching Access

```
vec : Vector[I64] { 1; 2; 3 }
vec`1 # { // vec(1)
    value`some { lambda }
    `none { lambda }
}
```

Matching returns an Option[Ref[Type]]
