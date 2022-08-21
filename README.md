# FtnCompiler
![Logo](https://github.com/Dan-Sad/LexAnalizer/blob/master/LexAnalyzer/img/LogoFtnCompiler.png)
# Description
It's simple Fortran compiler, written in C. 

FtnCompiler generates code in assembly.
# Syntactic possibilities
FtnCompiler can parse basic syntactic:

Keywords
```fortran
program     end     int     double            
while       for     exit    do
if          then    else
```

Operators
```fortran
+     -     *     /     %     =
>     <     >=    <=    ==    !=
:     ::    (     )     .     ,
```

# Example

To find 5 factorial
```fortran
program test
int :: x, y, i
x = 1
i = 2
y = 5
test_loop : do while (i <= y)
  x = x * i
  i = i + 1
end do
end program test
```
It will generate assembly code:
```assembly
mov eax, 1
mov eax, 2
mov eax, 5
mov eax, 1
mov ebx, 2
imul eax, ebx
mov eax, 2
mov ebx, 1
add eax, ebx
mov eax, 2
mov ebx, 3
imul eax, ebx
mov eax, 3
mov ebx, 1
add eax, ebx
mov eax, 6
mov ebx, 4
imul eax, ebx
mov eax, 4
mov ebx, 1
add eax, ebx
mov eax, 24
mov ebx, 5
push eax
imul eax, ebx
mov eax, 5
mov ebx, 1
add eax, ebx
```
