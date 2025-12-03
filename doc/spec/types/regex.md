
# Regex

PCRE Regular Expressions

## Creating

```
Regex { flags; "subsitition"; "pattern" }

~ "pattern"

flags ~ "pattern"

falgs ~ "subsitition" ~ "pattern"
```

## Flags

```
Enum[U8] { `invalid : 0; `d : 1 << 0; `g : 1 << 1; `i : 1 << 2; `m : 1 << 3; `s : 1 << 4 }
```

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

