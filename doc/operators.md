
# Operators

## Variables

### : Define/Assign

Create and or update a vars value

```
var : statement

var`symbol : statement

var(accessor) : statement
```

### :: Shadow

Create a var with the same name as one in the parent scope

### Casting $

```
var : TYPE $ data
```

### Swapping <:>

Swap the contents of two variables

```
var <:> var
```

### Deep Assign $:

Ref types cannot be assigned and shared are copied by pointer, this makes a complete recursive copy

```
var_a $: var_b
var_a = var_b // false
var_a == var_b // true
```

## [Bitwise](./operators/bitwise.md)

## [Arithmetic](./operators/arithmetic.md)

## [Logic](./operators/logic.md)
