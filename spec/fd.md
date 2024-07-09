
# File Descriptor
<< Read/Write/Pipe fd\<\<fd/data; <<(fd;fd/data)

<< Read/Write/Pipe [fd and info] << [data or fd and info];

## Modes
fd << data: Read

data << fd: Write

fd << fd: Pipe

## Builtin Descriptors
0 STDIN

1 STDOUT

2 STDERR
