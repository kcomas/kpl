
# File Descriptor
<< Read/Write/Pipe fd\<\<fd/data; <<(fd;fd/data)

<< Read/Write/Pipe [fd and info] << [data or fd and info];

## Modes
fd << data: Read

data << fd: Write

fd << fd: Pipe

## Builtin Descriptors
f0: STDIN

f1: STDOUT

f2: STDERR
