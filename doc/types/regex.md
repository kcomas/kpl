
# Regex

PCRE Regular Expressions

## Creating

```
Regex ( "pattern"; "subsitition"; flags )
```

## Using

String ~ "pattern"

String ~ Regex ( "pattern"; flags )

String ~ Regex ( "pattern"; "subsitition"; flags )

## Flags

See .regex.Flags

### d

Add indices to matches

### g

Global

### i

Case insensitive

### m

Make ^ $ match line start and end

### s

. matches newline
