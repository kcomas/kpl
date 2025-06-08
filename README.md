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
make DFLAGS="" OO=-O3
```

##### Build Flags

OO=-g -Og - Set optimization level

##### Debug Flags (DFLAGS=)

-DKPL_ALD=1 - Track allocations and frees

##### Option Flags (FLAGS=)

### Run

File

```bash
./kpl path/to/file
```

REPL

```bash
./kpl
```

##### Examples are in the ex folder
