
# Qualifiers

Type modifications

### Const[Type]

Anything within the container cannot be modified

### Ref[Type]

Reference, cannot be assigned

### Shared[Type]

Reference counting, self referential types use tracing garbage collection, cannot be used with threads

### Mutex[Type]

Assignable counted reference with lock, cannot be self referential, shared for threading
