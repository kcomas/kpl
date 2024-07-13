
# Types
Void: VD

Number types represent signed, unsigned and floats by 2^x eg u5 is unsigned 32bit integer

Integer: I3, I4, I5, I6, U3, U4, U5, U6

Float: F5, F6

String: SG Utf8 string "abc"

Symbol: SL c-string \`abc used in hashes and structs

Vector: VR<type> [1;2;3] vector of uniform types

Tuple: TE<type1;type2;....typen> ["a";1;3.14] fixed array of different types

Hash: HH<type> {\`a:1;\`b:2} hashmap of uniform types

Struct: ST<key1:type1;...keyn:typen> fixed hash of different types

Union: UN<st> only allocates enough space for largest type #TODO

Function: FN<argtype1;...argtypen;returntype> {[x;y] x+y}

Bound Function: BF<fn> {[x;y] x + y }(1;) > {[y] 1 + y} type cannot be explicitly created

Error: ER 'Error Message'

File Descriptor: FD
