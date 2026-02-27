# Control Flow

## If

### If Then

```
condition ? statement/lambda
```

### If Then Else

```
condition ? (if statement/lambda; else statement/lambda)
```

### Else If Else

```
? (
    condition ? statement/lambda
    ...
    else statement/lambda
)
```

## Loop

```
condition @ statement/lambda

loopable @ { TYPE?.value; TYPE?.index; statements }
```

### Break

### Continue

### Named Loops

```
condition @ {
    condition @ {
        .name\break
    }
}.name
```

## Match

```
matchable # (
    { TYPE?.var }.name // var is what has been matched
    ...
)
```
