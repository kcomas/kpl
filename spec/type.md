
# Types
Void: vd

Bool: bl can only be zero or 1

Number types represent signed, unsigned and floats by 2^x eg u5 is unsigned 32bit integer

Integer: i3, i4, i5, i6, u3, u4, u5, u6

Float: f5, f6

Char: cr Utf8 char "a"

String: sg Utf8 string "abc"

Symbol: sl c-string \`abc used in hashes and structs

Vector: vr<type> [1;2;3] vector of uniform types

Tuple: te<type1;type2;....typen> ["a";1;3.14] fixed array of uniform types

Hash: hh<type> {\`a:1;\`b:2} hashmap of uniform types

Struct: st<key1:type1;...keyn:typen> fixed hash of different types

Union: un<st> only allocates enough space for largest type

Function: fn<argtype1;...argtypen<returntype>> {(x;y) x+y}

Bound Function: bf<fn> {(x;y) x + y }(1;) > {(y) 1 + y} type cannot be explicitly created

Error: er 'Error Message'

File Descriptor: fd
