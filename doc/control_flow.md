
# Control Flow

## If

```
condition or { lambda }  ? statement or { lambda }

var : condition or { lambda } ? (
    statement or { lambda } then
    statement or { lambda } else
)

var : ? (
    condition or { lambda } ? statement or { lambda } then
    ...
    statement or { lambda } else
)
```

## Loop

### Labels

{}\`symbol can be used as a label to name the loop in the scope

```
condition or { lambda } @ statement or { lambda } or { lamba }`label

loopable @ statment or { lambda } or { lambda }`label
```

### \break

#### \`label\break

### \continue

#### \`label\continue

## Match

```
var : matchable # (
    { TYPE`var lambda }`symbol
    ...
)
```
