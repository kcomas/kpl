# KPL (Work In Progress)

## Typed/Jitted Scripting Language Written In C

### Supported Platforms

#### x64 Linux

### Dependencies

#### GNU Make

#### GCC

### Build

Debug

```bash
make
```

Optimized

```bash
make DFLAGS="" OO=-O1
```

##### Build Flags

OO=-g -Og - Set optimization level (-O2 currently will segfault)

##### Debug Flags (DFLAGS=)

-DKPL_ALD=1 - Track allocations and frees

##### Option Flags (FLAGS=)

### Run

```bash
./kpl path/to/file
```

##### Examples are in the ex folder
