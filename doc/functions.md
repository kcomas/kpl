# Functions

## Syntax

```
fn : I64 { I64.x + 1 }
fn : { I64.x + 1 } // same as above
fn(2)\log // I64 : 3
```

## Returning

### Implicit Return

### \return

### \ok

### \error

## Type Definition

Fn[Return Type; TYPE.name; ...]

## Type Inference

### Return Type

### Arguments

## Calling By Tuple

```
fn : { F64.x + F64.y }
args : Tupple(1.0; 2.0)
*(fn; args)\log // F64 : 3.0
```

## Overloading
