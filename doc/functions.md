
# Functions

## Returning

### Implicit return

### Keyword return

### Result with value $

### Result with error '

## C Compatible

Functions can be assigned to vars and cannot non function or namespace variables outside it's scope

```
var :: Fn[Return Type] { TYPE`arg1; ... body }
```

### Return Type Inference

If the return type is omitted it will be inferred

Recursive functions cannot be inferred and must have the return type

### Overloading

Chooses function based on args

```
var :: Overload { Fn[Type] { Body }; Fn[Type] { body } ... }
```

### Thread

## Lambda

Can access variables outside it's scope

Cannot be exported

```
var :: Lambda[Return Type] { Type`arg; ... body }

var :: { Type`arg; ... body }
```

### Closure

## Coroutine
