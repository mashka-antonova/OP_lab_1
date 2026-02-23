#ifndef LOGIC_H
#define LOGIC_H

#include <stddef.h>
#include <stdint.h>
#include "appcontext.h"
#include "entrypoint.h"

#define INPUT_BASE 10
#define OUTPUT_BASE 2
#define MIN_BASE 2
#define MAX_BASE 36
#define DECIMAL_BASE 10

void clearError(AppContext* context);
void setError(AppContext* context, Status code);
void initContext(AppContext* context);
void setInput(AppContext* context, const char* str);
int validateBase(int base);
int charToDigit(char c);
int validateStringForBase(const char* str, int base);
uint32_t parseToUint32(const char* str, int base, int* overflow);
void validateAll(AppContext* context);
void clearResult(AppContext* context);
void swap(AppContext* context);
void doInit(AppContext* context);
void doInputChanged(AppContext* context, const AppParams* params);
void doConvertClicked(AppContext* context, const AppParams* params);
void doSwapClicked(AppContext* context);

#endif // LOGIC_H
