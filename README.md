# KPL - Kevin's Programming Language

## Typed/Jitted Scripting Language written in C

## Supported Platforms and Dependencies

X86-64 Linux

GCC

GNU Make

## Build

Builds are done in the current working directory

```
git clone https://github.com/kcomas/kpl.git
cd kpl
make
```

## Usage

```
./kpl [opts] [file] # Examples with an overview can be found in the ex folder

[opts] are
-h: show help message

-d[steps]: show debugging output from p(psr) a(ast) f(fld) c(chk) o(opt) g(gen) s(as)
eg ./kpl -dps ./ex/ack.kpl # show the paser and assembler output for the file ./ex/ack.kpl
```

#### Example

```
./kpl ./ex/fib.kpl
```

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
