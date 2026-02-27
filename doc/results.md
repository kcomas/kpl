# Results

Temporary container for an value/error pair, cannot be assigned

## Errors

## Take \`

On failure the error is returned to the parent function

```
value : ` result
value "Error occured" ` result
```

## Match

```
fn : Result[Void] { "Error Message"\error }
fn : { "Error Message"\error } // same as above
value : fn() # (
    { I64.value }.ok
    {
        Error.error\log // String : "Error Message"
        1234
    }.error
)
value\log // I64 : 1234
```

### Ok

### Error
