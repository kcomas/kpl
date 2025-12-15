
# Enum

Map symbols to values of underlying type

```
My_enum : Enum[U8; `a : 1; `b : 2]
x : My_enum`b
x\log Enum[U8] `b 1
```

## Number Types

Defaults from zero if no number is specified

If number is specified, count after number

### \bitmask

Takes a number type enum with no number specified and treats each index after the first as a bit shift

### Multiple Enums at once

When using multiple symbols on an enum at once, the underlying type is a binary or of the underlying values

```
Example_enum : Enum[U8; `a : 0; `b : 1; `c : 1 << 1]
value : Example_enum`b`c
value\log // Enum[U8] `b`c 3
```
