
# X64

X86-64 Assembler/Disassembler

## Limitations

No access to segment registers

No X87 Support

## Required Implicit Registers

Instructions that implicitly modify a register require the register as an argument

```
# GAS Example
imul %rdi -> imul %rax, %rdi
leave -> leave %rbp
```

## Operation

## Dependency Graph

```
def -> inst
inst -> table
table -> mem
mem -> queue
queue -> state
state -> query
query -> asm
asm -> dis
dis -> x64
```

## Jit Code Layout

Text
Disassembler Byte
Signed 32bit data size
Data
