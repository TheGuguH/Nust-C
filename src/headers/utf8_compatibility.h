#ifndef UTF8_COMPATIBILITY_H
#define UTF8_COMPATIBILITY_H 

#include <stdint.h>
#include <stdlib.h>

/*
    Nust C char has 4 bytes, and EVERY character in a string in Nust C has 4 bytes, its a rule!
    Because this we need to make a algorithm for this conversion, using the table above this comment.
    We call the converted char a rune.
*/

typedef uint32_t rune_t;

/*
    Nust C string is a sequence of runes, called "Rune String" in NCT.
*/

typedef struct {
    rune_t *runeSequence;
    size_t runeQuantity;
} RuneString;

/*
    Unicode of a byte, just for limit verification
*/

extern const uint8_t CONTINUATION_VERIFY_BYTE;
extern const uint8_t CONTINUATION_BYTE;
extern const uint8_t UTF_2_BYTE_LENGHT_VERIFY_BYTE;
extern const uint8_t UTF_2_BYTE_LENGHT_BYTE;
extern const uint8_t UTF_3_BYTE_LENGHT_VERIFY_BYTE;
extern const uint8_t UTF_3_BYTE_LENGHT_BYTE;
extern const uint8_t UTF_4_BYTE_LENGHT_VERIFY_BYTE;
extern const uint8_t UTF_4_BYTE_LENGHT_BYTE;

extern const uint8_t ASCII_LIMIT;
extern const uint32_t BYTE_2_MINIMUM; 
extern const uint32_t BYTE_3_MINIMUM; 
extern const uint32_t BYTE_4_MINIMUM; 
extern const uint32_t UTF_START_SURROGATE_PAIRS; 
extern const uint32_t UTF_END_SURROGATE_PAIRS; 
extern const uint32_t UTF_8_LIMIT; 

RuneString* rs_create();

void rs_free(RuneString *string);

void rs_add(RuneString *string, rune_t rune);

void rs_addChar(RuneString *string, char _chars[], size_t _chars_s);

char* rs_convertToString(RuneString *_runeString);

rune_t rt_create(unsigned char _chars[], size_t _chars_s);

char* rt_convertToChar(rune_t rune);

void rt_print(rune_t rune);

void rt_println(rune_t rune);

int charUTF8Lenght(unsigned char _char);

void uPrint(char string[]);

void uPrintError(char string[], int errorCode);

#endif
