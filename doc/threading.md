
# Threading

Functions can be turned into threads

## Starting *

```
thread : args & fn
```

Args is moved into thread an cannot be accessed until joining, unless it is of type Mutex

## Joining &

```
value : "Error" ^ & thread
```

## Detaching &

When the thread is not assigned it starts in a detached state

```
args & fn
```

## Mutexes

Shared container for preventing data races
