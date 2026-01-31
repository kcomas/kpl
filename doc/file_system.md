
# File System

Files are closed when they go out of scope

```
open_file : Fd $ "filename"
```

## Buffers

Buffers are an intermediate type for reading and writing data, cannot be used directly

```
file_buffer : .io`read(Fd $ "filename")
.io`write(Fd $ "filename"; Buffer $ "Write this to file")
```

Buffers need to be manually cast to the type they intended to be used as

```
file_string : String $ .io`read(Fd $ "filename")
file_byte_vector : Vector[U8] $ .io`read(Fd $ "filename")
```
