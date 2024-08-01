#ifndef NUSTC_ERROR_CODES_H
#define NUSTC_ERROR_CODES_H

enum EErrorCode {
    // Memory errors
    ERROR_MALLOC_FAILED,
    ERROR_REALLOC_FAILED,
    ERROR_TRY_TO_FREE_NULL,
    ERROR_TRY_TO_USE_NULL,
    // Argument errors
    ERROR_NO_ENOUGH_ARGUMENTS,
    ERROR_INVALID_ARGUMENTS,
    // UTF-8 compatibility errors
    ERROR_UTF8_INVALID_LENGHT,
    ERROR_UTF8_NO_NEXT_BYTE,
    ERROR_UTF8_OVERLONG,
};

enum EWarnCode {
    WARN_EMPTY_FILE,
    WARN_EARLY_EOF,
};

void err_print(const enum EErrorCode code, const char *error, ...);

void err_warn(const enum EWarnCode code, const char *warn, ...);

void err_printExit(const enum EErrorCode code, const char *error, ...);

#endif