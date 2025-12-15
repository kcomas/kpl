
# Functions

## Returning

### Implicit return

### \return

### \value

### \error

## Default Values

## Call By Position

## Call By Symbol

## C Compatible

Functions can be assigned to vars and cannot non function or namespace variables outside it's scope

```
var : Fn[Return Type] { TYPE`arg1; ... body }
```

### Return Type Inference

If the return type is omitted it will be inferred

Recursive functions cannot be inferred and must have the return type

### Overloading

Chooses function based on args

```
var : Overload [ Fn[Type] { lambda }; Fn[Type] { lambda }; ... ]
```

## Lambda

Can access variables outside it's scope

Cannot be exported

```
var : Lambda[Return Type] { Type`arg; ... body }

var : { Type`arg; ... body }
```

### Argument Inference

Lambdas passed as functions can infer their args

```
map_example : { `x; x + 1 }  @ Array { 1; 2; 3}
map_example\log // Array[I64] { 2 3 4 }
```

### Tagged Lambda

```
{ lambda }`symbol }
```

The label is used for matching and named loops

### Closure

## Coroutine

### \yield
