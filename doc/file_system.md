
# File System

Files are closed when they go out of scope

```
open_file : Fd $ "filename"
```

## <& Read/Write

```
buffer <& Fd $ "filename"
buffer/log // Vector[U8]
string <& String $ <& Fd $ "filename"
$(Fd; "another_file") <& "Hello World\n"
```
