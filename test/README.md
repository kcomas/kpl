
# Testing

## Usage

Each test function is defined by a TEST macro

```
TEST(my_test) {
    // your test here
}
```

Each test name must be unique

### By default tests pass

### FAIL(MSG)

### ASSERT(STMT, FAIL_MSG)

## Run Test By Name

Tests names can be passed to the test exec and only those tests will run

Up to 64 names can be passed

## Directory Structure

### Example

### Lib

Tests for src/lib

## test.h

Only file tests include

## test.c

Testing entry point
