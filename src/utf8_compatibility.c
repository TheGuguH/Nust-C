#include "headers/utf8_compatibility.h"

#include "headers/error_codes.h"

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

/*
    We need a function to return a RuneString based on a character sequence
*/

void rt_print(rune_t rune) {
    unsigned char *runeBytes = (unsigned char*)&rune;

    for (size_t i = 0; i < sizeof(rune_t); i++) {
        printf("%c", runeBytes[i]);
    }
}

void rt_println(rune_t rune) {
    unsigned char *runeBytes = (unsigned char*)&rune;

    for (size_t i = 0; i < sizeof(rune_t); i++) {
        printf("%c", runeBytes[i]);
    }

    printf("\n");
}

RuneString* rs_convertTo(unsigned char charSequence[], size_t charSequenceSize) {
    RuneString *string = malloc(sizeof(RuneString));

    if (string == NULL) {
        puts("error on trying to alloc memory for a RuneString");
        exit(UTF8_ALLOC_ERROR);
    }

    string->runeSequence = calloc(1, sizeof(rune_t));

    if (string->runeSequence == NULL) {
        puts("error on trying to alloc memory for runeSequence of a RuneString");
        exit(UTF8_ALLOC_SEQUENCE_ERROR);
    }

    for (size_t i = 0; i < charSequenceSize; i++) {

        rune_t rune = rt_convertTo(charSequence, charSequenceSize, &i);

        string->runeQuantity++;

        string->runeSequence = realloc(string->runeSequence, sizeof(rune_t) * string->runeQuantity);

        if (string->runeSequence == NULL) {

            puts("error on trying to realloc memory for more capacity of runeSequence for a RuneString");
            exit(UTF8_REALLOC_SEQUENCE_ERROR);
        }

        string->runeSequence[string->runeQuantity - 1] = rune;
    }

    return string;
}

void rs_free(RuneString *string) {
    free(string->runeSequence);
    string->runeSequence = NULL;

    free(string);
    string = NULL;
}

rune_t rt_convertTo(unsigned char charSequence[], size_t charSequenceSize, size_t *index) {

    rune_t rune = 0;

    if (charSequence[*index] <= ASCII_LIMIT){
        rune = charSequence[*index];
        
        return rune;
    }

    VERIFT_INDEX_LIMIT(0);

    if ((charSequence[*index] & UTF_2_BYTE_LENGHT_VERIFY_BYTE) == UTF_2_BYTE_LENGHT_BYTE) { //is 2 byte-utf character lenght?
        VERIFT_INDEX_LIMIT(1);

        VERIFY_CONTINUATION_BYTE(1);

        SET_RUNE_T_BYTE(rune, '\0', '\0', charSequence[*index], charSequence[*index + 1]);

        *index += 1;

        VERIFY_LIMIT(rune, BYTE_2_MINIMUM);

    } else if ((charSequence[*index] & UTF_3_BYTE_LENGHT_VERIFY_BYTE) == UTF_3_BYTE_LENGHT_BYTE) { //is 3 byte-utf character lenght?
        VERIFT_INDEX_LIMIT(2);

        VERIFY_CONTINUATION_BYTE(1);
        VERIFY_CONTINUATION_BYTE(2);

        SET_RUNE_T_BYTE(rune, '\0', charSequence[*index], charSequence[*index + 1], charSequence[*index + 2]);

        *index += 2;

        VERIFY_LIMIT(rune, BYTE_3_MINIMUM);

    } else if ((charSequence[*index] & UTF_4_BYTE_LENGHT_VERIFY_BYTE) == UTF_4_BYTE_LENGHT_BYTE) { //is 4 byte-utf character lenght?
        VERIFT_INDEX_LIMIT(3);

        VERIFY_CONTINUATION_BYTE(1);
        VERIFY_CONTINUATION_BYTE(2);
        VERIFY_CONTINUATION_BYTE(3);

        SET_RUNE_T_BYTE(rune, charSequence[*index], charSequence[*index + 1], charSequence[*index + 2], charSequence[*index + 3]);

        *index += 3;

        VERIFY_LIMIT(BYTE_4_MINIMUM, rune); //By a unknown behavior, the inversion of datas

        if (rune < UTF_START_SURROGATE_PAIRS && rune > UTF_END_SURROGATE_PAIRS) {
            puts("trying to represent a surrogate pair");
            exit(UTF8_SURROGATE_PAIR);
        }

        if (rune < UTF_8_LIMIT) {
            puts("trying to represent over the UTF-8 limit");
            exit(UTF8_AWAY_LIMITS);
        }

        return rune;

    } else { //not a UTF-8 character!
        puts("A \"character\" to verify is not a UTF-8 character");
        exit(UTF8_INVALID_CHARACTER);
    }

    if (rune > UTF_START_SURROGATE_PAIRS && rune < UTF_END_SURROGATE_PAIRS) {
        puts("trying to represent a surrogate pair");
        exit(UTF8_SURROGATE_PAIR);
    }

    return rune;
}
