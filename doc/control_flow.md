
# Control Flow

## If

```
condition ? statement or { lambda }

var :: condition ? {
    statement or { lambda } then
    statement or { lambda } else
}

var :: ? {
    condition ? statement or { lambda } then
    ...
    statement or { lambda } else
}
```

## Loop

```
condition @ statement or { lambda }

{ variable; condition; each } @ statement or { lambda }
```

## Match

```
var :: matchable # {
    var`symbol { lambda }
    ...
}
```
