
# Result

Implicit error handling

128bit value/error pair, can only be checked not assigned

Automatically returns when an error occurs that is not checked

## Matching

```
var :: operation # {
    v`value { lamba }
    e`error { lambda }
}
```

## Checking '

```
var :: ERROR_DATA ' operation
```

Returns the ERROR_DATA with the error if there is an error
