
# X64

X86-64 Assembler/Disassembler

## Limitations

No access to segment registers

No access to upper 8bit of 16bit registers (ah, ch, dh, bh)

No X87 Support

## Required Implicit Registers

Instructions that implicitly modify a register require the register as an argument

```
# GAS Example
imul %rdi -> imul %rdi, %rax, %rdx
leave -> leave %rbp
```

For the order of implicit registers see [X64 Reference](http://ref.x86asm.net/coder64.html)

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

```
Text
Disassembler Byte
Signed 32bit data size
Data
```
