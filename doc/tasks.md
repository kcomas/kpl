# Tasks

All functions can be used asynchronously

## Creating

```
task : & fn(args)
```

### Detaching

```
& fn(args)
```

## Joining

```
value : & task
```

### Joining Multiple

```
values_tuple : & Tuple(task_a; task_b; task_c)
values_tuple(0)\log
(a; b; c) : & Tuple(task_a; task_b; task_c) // same as above
a\log // same as above
```

## Pipe

One producer and one consumer, cannot be shared

### Waiting

```
fn : { Consumer[Void].c(); "Task\n"\log }
pipe : Pipe[Void]()
& fn(` pipe.consumer)
// Stuff Here
pipe.producer()
// String : "Task" is printed to stdout and task exits
```

### Reading Data

```
fn : { Consumer[I64].c
    @ {
        x : c() // reading from consumer is a blocking call
        x # (
            {
                "Exit\n"\log
                \break
            }.0
            x\log
        )
    }
}
pipe : Pipe[I64]
p : ` pipe.producer
c : ` pipe.consumer
(p; c) ` Pipe[I64]/tuple // same as above
& fn(c)
p(1)
p(2)
p(0)
// Output
// I64 : 1
// I64 : 2
// String : "Exit\n"
```

### Type

Struct {
    Option[Producer[TYPE]].producer
    Option[Consumer[TYPE]].consumer
} Pipe[TYPE]
