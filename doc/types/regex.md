
# Regex

PCRE Regular Expressions

## Creating

```
Regex { pattern; substitution; flags }
```

## Flags

### d

Add indices to matches

### g

Global

### i

Case insensitive

### m

Make ^ $ match line start and end

### s

. matches newline

```
var : "pattern"\regex[_flags]?
Flag Combinations have to be in order
_dgims
_dims
_dgms
_dgis
_dgim
_gims
_gms
_gis
_gim
_ims
_is
_im
_ms
_m
_s
```
