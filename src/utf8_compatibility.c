#include "headers/utf8_compatibility.h"

#include "headers/error_codes.h"
#include "headers/utils.h"

#include <stdint.h>
#include <stdio.h>

/*
   Char. number range  |        UTF-8 octet sequence
      (hexadecimal)    |              (binary)
   --------------------+---------------------------------------------
   0000 0000-0000 007F | 0xxxxxxx
   0000 0080-0000 07FF | 110xxxxx 10xxxxxx
   0000 0800-0000 FFFF | 1110xxxx 10xxxxxx 10xxxxxx
   0001 0000-0010 FFFF | 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
*/

//
const uint8_t CONTINUATION_VERIFY_BYTE = 0b11000000;
const uint8_t CONTINUATION_BYTE = 0b10000000;
const uint8_t UTF_2_BYTE_LENGHT_VERIFY_BYTE = 0b11100000;
const uint8_t UTF_2_BYTE_LENGHT_BYTE = 0b11000000;
const uint8_t UTF_3_BYTE_LENGHT_VERIFY_BYTE = 0b11110000;
const uint8_t UTF_3_BYTE_LENGHT_BYTE = 0b11100000;
const uint8_t UTF_4_BYTE_LENGHT_VERIFY_BYTE = 0b11111000;
const uint8_t UTF_4_BYTE_LENGHT_BYTE = 0b11110000;

const uint8_t ASCII_LIMIT = 127;
const uint32_t BYTE_2_MINIMUM = 0xC280; 
const uint32_t BYTE_3_MINIMUM = 0xE0A080; 
const uint32_t BYTE_4_MINIMUM = 0xF0908080; 
const uint32_t UTF_START_SURROGATE_PAIRS = 0xEDA080;  //U+D800
const uint32_t UTF_END_SURROGATE_PAIRS = 0xEDBFBF;  //U+DFFF
const uint32_t UTF_8_LIMIT = 0xF48FBFBF; 

/*
    Nust C char has 4 bytes, and EVERY character in a string in Nust C has 4 bytes, its a rule!
    Because this we need to make a algorithm for this conversion, using the table above this comment.
    We call the converted char a rune.
*/

void rt_print(rune_t rune) {
    unsigned char *runeBytes = (unsigned char*)&rune;
    
    for (int i = 3; i >= 0; i--) {
        printf("%c", runeBytes[i]);
    }
}

void rt_println(rune_t rune) {
    rt_print(rune);
    puts("");
}

RuneString* rs_create() {
    RuneString *string = malloc(sizeof(RuneString));

    if (string == NULL) {
        uPrintError("can't alloc memory for a rune string", UTF8_ALLOC_ERROR);
    }

    string->runeSequence = malloc(sizeof(rune_t));

    if (string->runeSequence == NULL) {
        uPrintError("can't realloc memory for a rune sequence", UTF8_ALLOC_SEQUENCE_ERROR);
    }

    string->runeQuantity = 0;

    return string;
}

void rs_add(RuneString *string, rune_t rune) {
    string->runeSequence = realloc(string->runeSequence, (string->runeQuantity + 1) * sizeof(rune_t));

    if (string->runeSequence == NULL) {
        uPrintError("error on trying to realloc memory for runeSequence of a RuneString", UTF8_REALLOC_SEQUENCE_ERROR);
    }
    
    string->runeSequence[string->runeQuantity] = rune;

    string->runeQuantity++;
}

void rs_addChar(RuneString *string, char _chars[], size_t _chars_s) {

    rune_t rune = rt_create((unsigned char*)_chars, _chars_s);

    string->runeSequence = realloc(string->runeSequence, (string->runeQuantity + 1) * sizeof(rune_t));

    if (string->runeSequence == NULL) {
        uPrintError("error on trying to realloc memory for runeSequence of a RuneString", UTF8_REALLOC_SEQUENCE_ERROR);
    }
    
    string->runeSequence[string->runeQuantity] = rune;

    string->runeQuantity++;
}

void rs_free(RuneString *string) {
    free(string->runeSequence);
    string->runeSequence = NULL;

    free(string);
    string = NULL;
}

char* rs_convertToString(RuneString *_runeString) {
    char string_s = _runeString->runeQuantity * 4;
    char *string = calloc(string_s, sizeof(char));

    for (size_t i = 0; i < _runeString->runeQuantity; i++) {
        char *bytes = (char*)&_runeString->runeSequence[i];

        for (size_t j = 0; j < 4; j++) {
            string[i * 4 + j] = bytes[j];
        }
    }

    return string;
}

rune_t rt_create(unsigned char _chars[], size_t _chars_s) {
#define VERIFY_LENGHT(size)        if (_chars_s != size) \
            uPrintError("invalid lenght for this UTF-8 character", UTF8_INVALID_LENGHT); \

#define VERIFY_CONTINUATION_BYTE(index)       if ((_chars[index] & CONTINUATION_VERIFY_BYTE) != CONTINUATION_BYTE) { \
            uPrintError("one of bytes of a UTF-8 char ins't a continuation byte", UTF8_NO_CONTINUATION_BYTE); \
        } \

#define VERIFY_OVERLONG(minimum) if (rune < minimum) { \
            uPrintError("trying to make overlong", UTF8_OVERLONG); \
        } \

    rune_t rune = 0;

    switch (charUTF8Lenght(_chars[0])) {
        case 1:
            VERIFY_LENGHT(1);

            rune = _chars[0];
        break;
        case 2:
            VERIFY_LENGHT(2);

            VERIFY_CONTINUATION_BYTE(1);

            rune |= _chars[0];

            rune <<= 8;

            rune |= _chars[1];
            
            VERIFY_OVERLONG(BYTE_2_MINIMUM);
        break;
        case 3:
            VERIFY_LENGHT(3);

            VERIFY_CONTINUATION_BYTE(1);
            VERIFY_CONTINUATION_BYTE(2);

            rune |= _chars[0];
            rune <<= 8;

            rune |= _chars[1];
            rune <<= 8;

            rune |= _chars[2];

            VERIFY_OVERLONG(BYTE_3_MINIMUM);
        break;
        case 4:
            VERIFY_LENGHT(4);

            VERIFY_CONTINUATION_BYTE(1);
            VERIFY_CONTINUATION_BYTE(2);
            VERIFY_CONTINUATION_BYTE(3);

            rune |= _chars[0];
            rune <<= 8;

            rune |= _chars[1];
            rune <<= 8;

            rune |= _chars[2];
            rune <<= 8;

            rune |= _chars[3];

            VERIFY_OVERLONG(BYTE_4_MINIMUM);
        break;
        default:
            uPrintError("trying to represent a non UTF-8 char", UTF8_INVALID_CHARACTER);
        break;
    }

    if (rune > UTF_8_LIMIT) {
        uPrintError("trying to represent over the UTF-8 limit", UTF8_AWAY_LIMITS);
    }

    if (rune >= UTF_START_SURROGATE_PAIRS && rune <= UTF_END_SURROGATE_PAIRS) {
        uPrintError("trying to represent a surrogate pair", UTF8_SURROGATE_PAIR);
    }

    return rune;
}

int charUTF8Lenght(unsigned char _char) {
    if (_char <= ASCII_LIMIT){
        return 1;

    } else if ((_char & UTF_2_BYTE_LENGHT_VERIFY_BYTE) == UTF_2_BYTE_LENGHT_BYTE) { //is 2 byte-utf character lenght?
        return 2;

    } else if ((_char & UTF_3_BYTE_LENGHT_VERIFY_BYTE) == UTF_3_BYTE_LENGHT_BYTE) { //is 3 byte-utf character lenght?
        return 3;

    } else if ((_char & UTF_4_BYTE_LENGHT_VERIFY_BYTE) == UTF_4_BYTE_LENGHT_BYTE) { //is 4 byte-utf character lenght?
        return 4;

    } else { //not a UTF-8 character!
        return 0;
    }
}

void uPrint(char string[]) {
    printf("[UTF-8]: %s\n", string);
}

void uPrintError(char string[], int errorCode) {
    P_ERROR_FORMAT("[UTF-8]: ");
}
