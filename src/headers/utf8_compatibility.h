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
    Nust C string is a sequence of runes, called "Rune String" in NCC.
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

#define VERIFT_INDEX_LIMIT(limit)        if (charSequenceSize < (*index) + limit) { \
            puts("invalid index size for this UTF-8 character lenght"); \
            exit(UTF8_INVALID_INDEX_SIZE); \
        } \

#define VERIFY_CONTINUATION_BYTE(extra_index)       if ((charSequence[*index + extra_index] & CONTINUATION_VERIFY_BYTE) != CONTINUATION_BYTE) { \
            puts("a byte of a \"character\" ins't a continuation byte"); \
            exit(UTF8_NO_CONTINUATION_BYTE); \
        } \

#define SET_RUNE_T_BYTE(rune, first_value, second_value, third_value, fourth_value) unsigned char *runeBytes = (unsigned char*)&rune; \
 \
        runeBytes[0] = first_value; \
        runeBytes[1] = second_value; \
        runeBytes[2] = third_value; \
        runeBytes[3] = fourth_value; \

#define VERIFY_LIMIT(rune, limit)        if (rune < limit) { \
            puts("overlong of trying a representation of a UTF-8 character"); \
            exit(UTF8_OVERLONG); \
        } \

void rt_print(rune_t rune);

void rt_println(rune_t rune);

RuneString* rs_convertTo(unsigned char charSequence[], size_t charSequenceSize);

void rs_free(RuneString *string);

rune_t rt_convertTo(unsigned char charSequence[], size_t charSequenceSize, size_t *index);

#endif
