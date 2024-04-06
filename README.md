# Nust C

<br>

<div align="center">
<img src="images/logo/nust_c_logo-256.png" width="128" alt="Nust C logo with 256 pixels of size">
</div>

<br>
<br>

Nust C is a mid-level programming language that supports structured, functional, and a custom paradigms. Its typing is static and strong, meaning that the type of a variable needs to be defined, and there's no way to define a type with another incompatible type without directly manipulating memory.

The language also allows memory manipulation, dynamic memory allocation, security memory manipulation with a memory safe style.

The language have a algorithmics for detect memory leaks, in compiling, and has limit detection for execution time for security and normal types.

## Types

The language supports various primitive and special types, including:

### Primitive Types:

* **i1**: an integer type that occupies 1 byte in memory, accepting numbers from -128 to 127. Its equals to **byte**.
* **i2**: an integer type that occupies 2 bytes in memory, accepting negative and positive numbers. Its equals to **short**.
* **i4**: an integer type that occupies 4 bytes in memory, accepting negative and positive numbers. Its equals to **int**.
* **i8**: an integer type that occupies 8 bytes in memory, accepting negative and positive numbers. Its equals to **long**.
* **i16**: an integer type that occupies 16 bytes in memory, accepting negative and positive numbers.


* **f4**: a floating-point type that occupies 4 bytes in memory, accepting negative and positive numbers. Its equals to **float**.
* **f8**: a floating-point type that occupies 8 bytes in memory, accepting negative and positive numbers. Its equals to **double**.
* **f16**: a floating-point type that occupies 16 bytes in memory, accepting negative and positive numbers.

* **char**: a type that represents a character, currently only supporting ASCII characters.
* **bool**: a type that accepts true and false values.

### Special Types:

* **string**: a type that represents a sequence of characters, supporting only ASCII characters.
* **bitfield**: a type that occupies 1 byte in memory, representing a programming technique called bit field, where each bit of a byte or another type is used as a bool. In the case of Nust C, each bit can be accessed with the character # followed by the bit index (0 to 7). Each bit represents a bool.

#### Decimals:

Decimals are a special type, which although apparently work like a double, but more precise, they use a technique totally different from them, being a more precise technique

* **d4**: decimal that occupies 4 bytes in memory, extremely small.
* **d8**: decimal that occupies 8 bytes in memory. Its equals to **decimal**.
* **d16**: decimal that occupies 16 bytes in memory.

*Math with type 'decimal' is with the std/decimal_math standard library, and with the 'd16' is with std/precise_math*

Decimals are divided into 3 parts (and is in this order):
* **Sign flag**: tells whether or not it is a negative number, with 0 being positive and 1 being negative.
* **Decimal index**: carries the size of the decimal digits in bits, example: the number 8 (1000) is 4 in bits. The size is 5 in d4, 6 in d8, and 7 in d16.
* **Value part**: represents the value itself, here both the decimal digits and the whole part are mixed and are differentiated by the decimal index. The size is 26 in d4, 57 in d8, and 120 in d16.

*The decimal index represens the size of decimal part (after the '.') in bits, i. e., the "value part" is separed in integer and decimal part. The sign flag represents the signal, if is 0, the signal is positive, if is 1, the signal is negative.*

*Also, note that there are no unsigned decimal types!*

### Modifiers

Modifiers are keywords that modify a variable or a member of a structure.

#### Signed
The signed keyword allows signs in integer variables (byte, short, int and long), decreasing the maximum value in 2 consequently.

*all integer types are signed, with the exception of byte*

Example:
signed byte number = -127;

#### Unsigned

The unsigned keyword does not allow signs in integer variables (byte, short, int and long), increasing the maximum value accordingly.

*the byte type is naturally unsigned*

Example:
unsigned int number = 3000000000;

#### Unsafe

The unsafe keyword make the variable dynamic, that is, the programmer need to allocated, liberted and reallocated the memory for this variable. The memory safe of language don't work for this, only possible memory leaks is used on .nclog file for compilations.

*don't have one natural unsafe type*

unsafe int number; //this need to be allocated

#### Safe

This keyword mark the variable or member safety, that is, the memory safe work well for this, and have useful logs on **.nclog** for possible errors on handling memory with these types.

safe int number = 2000;

#### Security

This keyword marks the variable or member as a type for security purposes, i.e., has checks for maximum security, and has useful logs in **.nlog** for possible errors in memory handling with these variable. These variable force you to write safer codes.

*all types is security normally, this keyword can be omited*

security int number = 200;

*this modifier are inspired in normal Rust types*

#### Imut

This keyword marks the variable or member as a imutable type, i. e., the value can't be change without shadowing.

*all local variable of instruction blocks whether or not inside local structures (functions, behaviors, sub-rotines, sub-behaviors and procedures), or global variable, are naturally imut*

imut int number = 25;
number += 25; //not possible because is imutable

#### Mut

This keyword marks the variable or member as a mutable type, i. e., the value can change without shadowing.

*all local variables of member structures (not for local variables for behaviors, sub-behaviors or procedures) are naturally mut for internal purposes*

mut int number = 20;
number += 20; //possible

#### Global

Marks the variable as unique in the program, and only file scope variables not linked to instruction blocks or structures can be marked as "global".

*especially useful with the next keyword or variables that can and should be accessed anywhere in the program*

global const double PI = 3.1415;

#### Const

It marks the variable as a constant in the program, that is, it cannot be modified, and is lighter and more efficient than immutable variables in terms of eliminating "magic numbers".

*especially useful with the "global" keyword*

global const double PI = 3.1415;

#### Volatile

This keyword defines a variable in a specific memory location, allowing it to be changed externally, that is, outside the program. Because this, is more dangerous than normal variables.

*extremely useful in embedded systems like Arduino*

volatile bool *digitalPort1 = (bool *)0xff04ff01; //an example of a memory location