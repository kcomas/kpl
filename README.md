# KPL - Kevin's Programming Language

## Typed/Jitted Scripting Language written in C

## Supported Platforms and Dependencies

X86-64 Linux

GCC

GNU Make

## Build

Builds are done in the current working directory

```
make
```

## Usage TODO

#### Examples with an overview can be found in the ex folder

## Tests

All Tests:

```
make tests
./tests
```

Individual tests:

```
make show_tests # List of test names
make $(TEST_NAME)
./$(TEST_NAME)
```

## Clean

To remove the current build or tests

```
make clean
```
