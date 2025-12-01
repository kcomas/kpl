
# Result[Type]

Implicit error handling

128bit value/error pair, can only be checked

Not assignable, only returnable

Automatically returns when an error occurs that is not checked

## Assigning

```
var :: result
```

Assigned the value to the var, returns if error

## Matching

```
var :: operation # {
    var_value`value { lamba }
    var_error`error { lambda }
}
```

## Checking ^

```
var :: ERROR_DATA ^ operation
```

Returns the ERROR_DATA with the error if there is an error

## Returning

### ^ Value

### Error ^
