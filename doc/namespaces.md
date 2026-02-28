# Namespaces

Use code across files

## Exporting

file.kpl

```
fn : { I64.x + 1 }
fn\export
```

## Importing

import.kpl

```
ns : "./file.kpl"\import
ns.fn(2)\log // I64 : 3
```

### Loading Exported Directly

import.kpl

```
"./file.kpl"\import [ .fn ]
ns.fn(2)\log // I64 : 3
```

#### Renaming Imports

import.kpl

```
"./file.kpl"\import [ import_fn : .fn ]
import_fn.fn(2)\log // I64 : 3
```

## Exporting Files In Other Files

file2.kpl

```
file : "./file.kpl"\import
file\export
```

#### Using Re Exported Files

import.kpl

```
ns : "file2.kpl"\import
ns.file.fn(2)\log // I64.3
```
