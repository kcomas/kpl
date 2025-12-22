
# Threading

Functions can be turned into threads

## Starting *

```
thread : args * fn
```

Args is moved into thread an cannot be accessed until joining, unless it is of type Mutex

## Joining &

```
value : "Error" ^ & thread
```

## \`args

Once the thread is completed the args passed into the thread can be access on the thread via symbol

Access args returns an Option[Ref[Type]]

## \`status

.thread.Status

## Mutexes

Shared container for preventing data races
