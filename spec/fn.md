
# Functions
Implicit and explicit return

x:FN(i6;i6;i6)${(x;y) x+y} define a function with its type

x:FN(i6;i6;i6)[;|\n] define a type without a function

x:{(x;y) x+y} define a function and infer its type on calls

#; return {(x) #; x+1; 2 } returns x+1

S: Self Call, Recursion And Assignment f:{(x) S(x+1) } , f:{(x) f(x+1) } f will not be defined
