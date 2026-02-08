
# Task (Threading)

Functions can be turned into threads

## Starting &

```
thread : args & fn
thread_no_args & fn
```

Args is moved into thread an cannot be accessed until joining

Values in args that are type mutex can be accessed outside the thread

## Joining

The join always returns a Result[Struct[ARGSTYPE\`args; RETURNTYPE\`return]]

```
value : thread
value : "Error" ^ thread
```

If the thread has no data join with the unused var

```
_ : thread
```

## Detaching &

When the thread is not assigned it starts in a detached state

```
args & fn // detatched with args
& fn // detatched no args
```

## Mutexes

Shared container for preventing data races
