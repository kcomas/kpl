# C Code

Use native functions

## C Types

C\_fn[Return Type; TYPE.name; ...]

C\_data

## Registering

C Code

```
int64_t add(int64_t a, int64_t b) {
    return a + b;
}

// TODO registration code
```

## Using

import.kpl

```
lib : "lib"\use
lib.add(1; 2)\log // I64 : 3
```

### Renaming Imports

import.kpl

```
"lib"\use [ .add ]
add\log // C_fn[I64; I64.a; I64.b]
add(1; 2)\log // I64 : 3
```
