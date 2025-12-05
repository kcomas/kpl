
# File System

Files are closed when they go out of scope

```
open_file : Fd $ "filename"
```

## << Read/Send

```
buffer << Fd $ "filename"
buffer/log // Vector[U8]
string << String $ << Fd $ "filename"
$(Fd; "another file") << "Hello World\n"
```
