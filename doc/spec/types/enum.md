
# Enum

Map symbols to values of underlying type

```
My_enum : Enum[U8] { `a : 1; `b : 2 }
x : My_enum`b
x\log Enum[U8] `b 1
```

## Number Types

Defaults from zero if no number is specified

If number is specified, count after number

### \bitmask

Takes a number type enum with no number specified and treats each index after the first as a bit shift
