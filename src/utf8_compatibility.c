#include "utf8_compatibility.h"

#include "error.h"

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
const uint8_t CONTINUATION_VERIFY_BYTE = 0xc0; //0b11000000
const uint8_t CONTINUATION_BYTE = 0x80; //0b10000000
const uint8_t UTF_2_BYTE_LENGHT_VERIFY_BYTE = 0xe0; //0b11100000
const uint8_t UTF_2_BYTE_LENGHT_BYTE = 0xc0; //0b11000000
const uint8_t UTF_3_BYTE_LENGHT_VERIFY_BYTE = 0xf0; //0b11110000
const uint8_t UTF_3_BYTE_LENGHT_BYTE = 0xe0; //0b11100000
const uint8_t UTF_4_BYTE_LENGHT_VERIFY_BYTE = 0xf8; //0b11111000
const uint8_t UTF_4_BYTE_LENGHT_BYTE = 0xf0; //0b11110000

const uint8_t ASCII_LIMIT = 127;
const uint32_t BYTE_2_MINIMUM = 0xc280; 
const uint32_t BYTE_3_MINIMUM = 0xe0a080; 
const uint32_t BYTE_4_MINIMUM = 0xf0908080; 
const uint32_t UTF_START_SURROGATE_PAIRS = 0xeda080;  //U+D800
const uint32_t UTF_END_SURROGATE_PAIRS = 0xedbfbf;  //U+DFFF
const uint32_t UTF_8_LIMIT = 0xf48fbfbf; 

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
        uPrintError("can't alloc memory for a rune string", ERROR_MALLOC_FAILED);
    }

    string->runeSequence = malloc(sizeof(rune_t));

    if (string->runeSequence == NULL) {
        uPrintError("can't realloc memory for a rune sequence", ERROR_MALLOC_FAILED);
    }

    string->runeQuantity = 0;

    return string;
}

void rs_add(RuneString *string, rune_t rune) {
    string->runeSequence = realloc(string->runeSequence, (string->runeQuantity + 1) * sizeof(rune_t));

    if (string->runeSequence == NULL) {
        uPrintError("error on trying to realloc memory for runeSequence of a RuneString", ERROR_REALLOC_FAILED);
    }
    
    string->runeSequence[string->runeQuantity] = rune;

    string->runeQuantity++;
}

void rs_addChar(RuneString *string, char _chars[], size_t _chars_s) {

    rune_t rune = rt_create((unsigned char*)_chars, _chars_s);

    string->runeSequence = realloc(string->runeSequence, (string->runeQuantity + 1) * sizeof(rune_t));

    if (string->runeSequence == NULL) {
        uPrintError("error on trying to realloc memory for runeSequence of a RuneString", ERROR_REALLOC_FAILED);
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
    if (_chars_s == 0)
        uPrintError("invalid lenght for this UTF-8 character", ERROR_UTF8_INVALID_LENGHT);

    rune_t rune = 0;

    size_t size = charUTF8Lenght(_chars[0]);

    if (_chars_s != size)
            uPrintError("invalid lenght for this UTF-8 character", ERROR_UTF8_INVALID_LENGHT);

    for (size_t i = 0; i < _chars_s - 1; i++) {
        rune |= _chars[i];
        rune <<= 8;

        if ((_chars[i + 1] & CONTINUATION_VERIFY_BYTE) != CONTINUATION_BYTE) {
            uPrintError("one of bytes of a UTF-8 char ins't a continuation byte", ERROR_UTF8_NO_NEXT_BYTE);
        }
    }

    rune |= _chars[_chars_s - 1];

    uint32_t minimum;

    if (_chars_s == 1)
        return rune;
    else if (_chars_s == 2)
        minimum = BYTE_2_MINIMUM;
    else if (_chars_s == 3)
        minimum = BYTE_3_MINIMUM;
    else
        minimum = BYTE_4_MINIMUM;

    if (rune < minimum) {
        uPrintError("trying to make overlong", ERROR_UTF8_OVERLONG);
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
        printf("[UTF-8]: %s\n", string);
}
