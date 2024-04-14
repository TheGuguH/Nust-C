# Coding Style

This *.md* includes the coding style of the Nust C compiler, but if you want to know the coding style of the Nust C standard libraries, go to the "lib" folder, there it contains the coding style of the standard libraries. There is also the style for *.md* used from wikipedias and separate *.md* like these.

## Names

***Always use a descriptive name!***

### '_t' typedef sufix 

The use of the '_t' suffix when using typedef is only applied to "pure" typedefs, that is, typedefs that are not followed by a definition of a structure such as struct, union or enum. Example of an use of the suffix:

```c
typedef uint32_t rune_t;
```

An example of where it cannot be used:

```c
typedef struct {
    rune_t *runeSequence;
    size_t runeQuantity;
    size_t allocatedMemory;
} RuneString;
```

### Structures

Structure names (struct, union and enum) use PascalCase, with the exception of the enum which uses EPascalCase; the enum rarely has an associated name.

```c
typedef struct {
     rune_t *runeSequence;
     size_t runeQuantity;
     size_t allocatedMemory;
} RuneString;
```

Comments:
- Structures that have an associated name must always be one with typedef.
- If the structure needs to have itself within it (as in a simple linked list), the name of the structure will be the same. Example:

```c
typedef struct LinkedNode {
     void *data;
     struct LinkedNode *next;
} LinkedNode;
```

### Variables identifiers

Variable identifiers use the camelCase pattern. Example:

```c
int foo = 25;
```

#### Suffixes and variable prefixes

Variable suffixes are intended to show what that variable has. Remembering that the suffixes here are just to help when reading and writing the code, ignoring extremely repetitive and often unnecessary things like the "prt" prefix in some styles.

- **s**:
     This suffix is normaly ussed in arrays. Exemple:

     ```c
         FILE files[20];
     ```
- **Buffer**:
     This suffix is only applied to buffers, and can also be used as "Buffers" if it is an array or list of buffers. Example:
     ```c
     int[5, 5] indexBuffers;
     ```
- **Index**:
     This suffix warns that it is an index variable; does not apply to for loops.

     ```c
         int fileIndex;
     ```
- **is, has, make and build**:
     These prefixes are only applied to variables that serve as a flag (boolean) for something. Example:

     ```c
         int hasBackground;
     ```
- **_c**:
     This sufix marks a variable as a count, and normaly this goes with a value variable (see next sufix). The default value for this is *size_t*, but for args of main function, the normal type is *int*. Exemple:

     ```c
         size_t file_c;
         FILE *file_v[];
     ```
- **_v**:
     This sufix marks a variable as a value, and normaly this goes with a count value (see previus sufix). Exemple:

     ```c
         size_t file_c;
         FILE *file_v[];
     ```
- **_s**:
     This sufix marks a variable as a size, and normaly this goes with a string. The default type is *size_t*. Exemple:

     ```c
         char *identifier;
         size_t identifier_s;
     ```
- **_**:
     This prefix is used just for blocking clonflits, like: types, struct, union and enum name or extension name conflicts (like the one from clangd that warns that the identifier "namespace" is defined in Objective-C++ since you are programming in C). These conflict considerations MUST not be case-sensitive and need to ignore *s* sufix. Exemple:
     
     ```c
         int isASCII(char _char) {
             //...
         }
     ```

### Pointers

The pointers two cases:

#### As a variable:

When used as a variable, use the '*' before the name, but attached. In this case is the variable, not the type, is the pointer. Example:

```c
void *stringLocation;
```

#### As a return:

When used as a return, the '*' is used after the type, but attached. Because in this case the return is the pointer. Example:

```c
rune_t* convertToRune(char[] characterString) {
     //...
}
```

### Function identifiers

CamelCase (camelCase) is used for function identifiers, and when referring to functions without returns, we use subroutine and not function to make it easier to speak and write; the same applies to functions without arguments and returns, we call procedures. The general name for function, subroutines and procedures is "instruction block".

When the instruction block is "*linked*" to a structure, we use an abbreviation of it followed by an _ and the name, example:

```c
typedef struct {
     LinkedNode *head;
     LinkedNode *tail;
     size_t size;
} SingleLinkedList;

SingleLinkedList* sll_create() {
     //...
}
```

Function example:

```c
rune_t* convertToRune(char[] characterString) {
      //...
}
```

Subroutine example:

```c
void sll_addItem(SingleLinkedList *list, void *data) {
     //...
}
```

Example procedure:

```c
void startDrawing() {
     //...
}
```

#### Instruction block suffixes

Instruction block suffixes are to indicate additional behavior on that instruction block.

- **_op**:
     It means that this instruction block has optional arguments, and that calling this function without this suffix will not need to specify these arguments. Example:

```c
double v2_getRotation(Vector2D vector) {
     //...
}

double v2_getRotation_op(Vector2D vector, Vector2D center) {
     //...
}
```
- **_np**:
     It means that this instruction block is **non pure**, that is, it uses variables shared globally or locally. Example:

```c
FILE[] filesToCompile;

void compileFiles_np() {
     //some use of the 'filesToCompile' array
}
```

### Constants

When we create a constant, SCREAMING_SNAKE_CASE is used; applies to macros, variables marked as const, or enums. Examples:

Const:

```c
const double PI = 3.141592653589793;
```

Macro:

```c
#define LINUX_EXECUTABLE_PREFIX .out
```

Enum:

```c
enum {
     FALSE,
     TRUE,
};
```
Note: **ALWAYS** after an enum value there is a ','.

## Syntax

These rules manage the way the code should be done!

### Separation of code blocks and instructions

When using '{', '}', mathematical operations, etc., each one must be separated; the initialization of a block of code or instruction must be on the same line. Example:

```c
int a = 3;
int b = 5;
int c = a + b;

void main() {
     int c = 2;

     printf("%d", c);
}
```

We always prefer a smaller instruction block that does one function than a giant instruction block that does multiple functions

### Instruction blocks

If the instruction block has arguments, use the parentheses attached with the identifier of that instruction block; the parameters must also be enclosed in parentheses; After the ',' a space is used. Example:

```c
void startDrawing(int anthropicBlurLevel, Color backgroundColor) {
     //...
}
```

If-else inversion is used when there is a lot of if nested in a function

### If-else, loops and switch-case

For all flow controllers, the '()' must be separated. Example:

```c
if (1 == 1) {
     return 10;
}
```

### White spaces

Always use whitespace to separate unrelated behaviors or blocks of instructions and code. Furthermore, it is recommended to separate the declaration of variables from the instructions. Example:

```c
int main() {
     int a = 25;

     printf("%d", a);

     return 0;
}
```

### Tabulation

Never use tabs, use 4 spaces instead

## Includes

Includes must always follow this pattern:

The first include will be that of that file, that is, if the file is called *lexer.c*, the first include will be of the header of that file, which whenever (again) is not an interface, will be the name of the file, replacing the extension by *.h*. Example:

```
lexer.c -> source code
lexer.h -> header
```

And in *lexer.c*:

```c
#include "lexer.h"
```

As a rule, headers are located in a '*header*' subfolder.

The next includes (separated by a blank line) are the local libs, always in alphabetical order. External libs must be located within a lib subfolder, and normally, one or more previous folders will be located if the project is separated into modules. Example:

```c
#include "headers/lexer.h"

#include "../lib/stb_image.h"
```

The next includes (separated again by a blank line) are the local files, always in alphabetical order. They are normally located in the "headers" subfolder, but if it is separated into modules, it may be necessary to use a subfolder. Example:

```c
#include "headers/lexer.h"

#include "../lib/stb_image.h"

#include "headers/utils.h"
```

The next includes (also separated by a blank line) are the standard C libs, always in alphabetical order. Example:

```c
#include "headers/lexer.h"

#include "../lib/stb_image.h"

#include "headers/utils.h"

#include <stdio.h>
```

- Generally utility files should be located in the same location as the project initialization file (main.c), that is, in this folder there should be the utility files, the initialization file and all the project modules

### Definition of constants and structures

Structures, macros (#define) and typedef must be declared in their header (.h) file, rather than in the source code (.c) directly.

Note that we prefer to use macros rather than generic instruction blocks that never change or that change one word or another, like variable types. Example:

```c
#define BYTE_1_VERIFY_UTF_8_SEQUENCE if ((charSequence[*index + 1] & utf_8SequenceByteVerifier) != utf_8SequenceByte) { \
             perror("second byte of a rune is incorrect!"); \
             exit(4); \
         } \
```

The definition of these structures, macros and typedef generally follows the following order:

```
enums

"pure" typedef

const

union
struct

macros

declaration of structure blocks
```
