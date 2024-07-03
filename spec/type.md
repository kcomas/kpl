
# Types
Void: vd

Bool: bl b0, b1

Integer: i8, i16, i32, i64, u8, u16, u32, u64

Float: f32, f64

Char: cr Utf8 char "a"

String: sg Utf8 string "abc"

Symbol: sl c-string \`abc used in hashes and structs

Vector: vr<type> [1;2;3] vector of uniform types

Tuple: te<type1;type2;....typen> ["a";1;3.14] fixed array of uniform types

Hash: hh<type> {\`a:1;\`b:2} hashmap of uniform types

Struct: st<key1:type1;...keyn::typen> fixed hash of different types

Union: un<st> only allocates enough space for largest type

Function: fn<argtype1;...argtypen<returntype>> {(x;y) x+y}

Bound Function: bf<fn> {(x;y) x + y }(1;) > {(y) 1 + y}

Error: er 'Error Message'

File Descriptor: fd
