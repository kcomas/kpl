# Control Flow

## If

If Then

```
condition ? statement

condition ? { lambda }
```

If Then Else

```
condition ? (
    true statement/lambda
    false statement/lambda
)
```

If Else If Else

```
? (
    condition ^ { lambda }
    condition ^ statement
    else statement/{ else lambda }
)
```

## Loop

```
condition @ statement/lambda
```

### Infinite Loop

@ statement/lamda

### \`break

### \`continue

### Named Loop

Loops can be named, allowing for breaking nested loops

## Match

```
matchable # (
    case ^ { lambda }
    case ^ statement
    no match statement/{ no match lambda }
)
```
