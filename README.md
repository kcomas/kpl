# KPL (Work In Progress)

## Typed/Jitted Scripting Language Written In C

### Supported Platforms

#### x64 Linux

### Dependencies

#### GNU Make
#### GCC

### Build

```bash
make
```
##### Debug Build Flags (DFLAGS)

ALD=1 - Track allocations and frees

OO=-g -Og -Set optimization level (-O2 currently will segfault)

### Run

```bash
./kpl path/to/file
```
##### Examples are in the ex folder
