#ifndef APPCONTEXT_H
#define APPCONTEXT_H

#include <stdint.h>

#define ERR_MESSAGE_SIZE 128

typedef enum{
    STATUS_OK,
    ERR_EMPTY_INPUT,
    ERR_INVALID_CHAR,
    ERR_INVALID_BASE,
    ERR_OVERFLOW,
    ERR_NO_MEMORY,
    ERR_INTERNAL

} Status;

typedef struct{
    int inputBase;
    int outputBase;

    Status errorCode;
    uint32_t parsedValue;

    char* inputNum;
    char* resultNum;

} AppContext;

#endif // APPCONTEXT_H
