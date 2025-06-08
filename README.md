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
##### Build Flags

OO=-g -Og - Set optimization level (-O2 currently will segfault)

##### Debug Flags (DFLAGS=)

-DALD=1 - Track allocations and frees

### Run

```bash
./kpl path/to/file
```
##### Examples are in the ex folder
