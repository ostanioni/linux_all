## [size_t](#size_t)
## [NULL](#NULL)

### size_t

	Defined in header <stddef.h>
	Defined in header <stdio.h>
	Defined in header <stdlib.h>
	Defined in header <string.h>
	Defined in header <time.h>
	Defined in header <uchar.h> (since C11)
	Defined in header <wchar.h> 
> `size_t` is the unsigned integer type of the result of `sizeof` , `_Alignof` (since C11) and offsetof, depending on the data model. The bit width of `size_t` is not less than 16. (since C99)`size_t` can store the maximum size of a theoretically possible object of any type (including array). `size_t` is commonly used for array indexing and loop counting. Programs that use other types, such as unsigned int, for array indexing may fail on, e.g. 64-bit systems when the index exceeds `UINT_MAX` or if it relies on 32-bit modular arithmetic. 


```c
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
 
int main(void)
{
    const size_t N = 100;
    int numbers[N];
    for (size_t ndx = 0; ndx < N; ++ndx)
        numbers[ndx] = ndx;
    printf("SIZE_MAX = %zu\n", SIZE_MAX);
    size_t size = sizeof numbers;
    printf("size = %zu\n", size);
}
```

Possible output:
```sh
SIZE_MAX = 18446744073709551615
size = 400
```

### NULL
	Defined in header <stddef.h>
	Defined in header <string.h>
	Defined in header <wchar.h>
	Defined in header <time.h>
	Defined in header <locale.h>
	Defined in header <stdio.h>
	Defined in header <stdlib.h> 
> #define NULL /*implementation-defined*/		

The macro NULL is an implementation-defined null pointer constant, which may be

    an integer constant expression with the value ​0​
    an integer constant expression with the value 0 cast to the type void* 

A null pointer constant may be converted to any pointer type; such conversion results in the null pointer value of that type.

**Possible implementation**

C++ compatible:
```c
		#define NULL 0
```
C++ incompatible:
```c
	#define NULL (10*2 - 20)
	#define NULL ((void*)0)
```
```c
#include <stdlib.h>
#include <stdio.h>
int main(void)
{    
    // any kind of pointer can be set to NULL
    int* p = NULL;
    struct S *s = NULL;
    void(*f)(int, double) = NULL;
 
    // many pointer-returning functions use null pointers to indicate error
    char *ptr = malloc(10);
    if (ptr == NULL) printf("Out of memory");
    free(ptr);
}
```

### ptrdiff_t
 
	Defined in header <stddef.h>
		
typedef /*implementation-defined*/ ptrdiff_t;
		
		

`ptrdiff_t` is the signed integer type of the result of subtracting two pointers.

The bit width of `ptrdiff_t` is not less than 17.
	(since C99)
*Notes*

`ptrdiff_t` is used for pointer arithmetic and array indexing, if negative values are possible. Programs that use other types, such as int, may fail on, e.g. 64-bit systems when the index exceeds `INT_MAX` or if it relies on 32-bit modular arithmetic.

Only pointers to elements of the same array (including the pointer one past the end of the array) may be subtracted from each other.

If an array is so large (greater than `PTRDIFF_MAX` elements, but less than `SIZE_MAX` bytes), that the difference between two pointers may not be representable as `ptrdiff_t`, the result of subtracting two such pointers is undefined.

For char arrays shorter than `PTRDIFF_MAX`, `ptrdiff_t` acts as the signed counterpart of `size_t`: it can store the size of the array of any type and is, on most platforms, synonymous with `intptr_t`).
Example
Run this code
```c
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
 
int main(void)
{
    const size_t N = 100;
    int numbers[N];
 
    printf("PTRDIFF_MAX = %ld\n", PTRDIFF_MAX);
    int *p1=&numbers[18], *p2=&numbers[23];
    ptrdiff_t diff = p2-p1;
    printf("p2-p1 = %td\n", diff);
 
    return 0;
}
```

Possible output:
```sh
PTRDIFF_MAX = 9223372036854775807
p2-p1 = 5
```

## `max_align_t`
 
	Defined in header <stddef.h>
		
typedef /*implementation-defined*/ `max_align_t`;
		(since C11)
		

`max_align_t` is a type whose alignment requirement is at least as strict (as large) as that of every scalar type.
Notes

Pointers returned by allocation functions such as malloc are suitably aligned for any object, which means they are aligned at least as strictly as `max_align_t`.

`max_align_t` is usually synonymous with the largest scalar type, which is long double on most platforms, and its alignment requirement is either 8 or 16.
Example
Run this code
```c
#include <stdio.h>
#include <stddef.h>
#include <stdalign.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
 
int main(void)
{
    size_t a = alignof(max_align_t);
    printf("Alignment of max_align_t is %zu (%#zx)\n", a, a);
 
    void *p = malloc(123);
    printf("The address obtained from malloc(123) is %#" PRIxPTR"\n",
            (uintptr_t)p);
    free(p);
}
```
Possible output:
```sh
Alignment of max_align_t is 16 (0x10)
The address obtained from malloc(123) is 0x1fa67010
```

