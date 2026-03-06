# Results

## Implicit Value

```
var : result // assign the value of the result to var
```

If result is error, the function will then return error with stack trace to parent function

## Checking For Error

```
result ? { .error }
```

## Matching

```
result # (
    .value ^ { .value ... }
    .error ^ { .error ... }
)
```

## Keywords

### Returning

#### \`value

#### \`error
