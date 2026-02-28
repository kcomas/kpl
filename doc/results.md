# Results

Temporary container for an value/error pair, cannot be assigned

## Errors

Wrapper around data

```
er : Error("Data")
```

### Errors are for reporting only, their data cannot be removed or used

## Take \`

On failure the error is returned to the parent function

```
value : ` result
value "Error occured" ` result
```

### In Function

Using results in a function

```
result_fn : { 1\ok }
fn: {
    x : result_fn() // this could error which would cause fn to return here
    x : "Error Occured ` result_fn() //  Same as above, but adding another error to the stack
    x\log // I64 : 1
    x\ok // cannot just return x have to return result
}
fn\log Fn[Result[I64]]
fn()\log // I64 : 1
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
