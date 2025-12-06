
# Option[Type]

May have value

## '

### Some ' Value

```
x :: ' 123
x\log // Option[I64] 123
```

### None '

```
x :: ' I64
x\log // Option[I64]
```

## Matching

```
var :: option # {
    var_some`some { lambda }
    `none { lambda }
}
```

## Checking ?

## Accessing

Options can be directly accessed according to their inner type, if the option is none an error is returned
