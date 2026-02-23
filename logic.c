#include "logic.h"
#include <stdlib.h>
#include <string.h>
#include "convert.h"

void clearError(AppContext* context) {
  if (context != NULL)
    context->errorCode = STATUS_OK;
}

void setError(AppContext* context, Status code) {
  if (context != NULL)
    context->errorCode = code;
}

void initContext(AppContext* context) {
  if (context != NULL) {
    context->inputBase = INPUT_BASE;
    context->outputBase = OUTPUT_BASE;
    context->inputNum = NULL;
    context->resultNum = NULL;
    context->parsedValue = 0;
    clearError(context);
  }
}

void setInput(AppContext* context, const char* str) { //можно передавать параметры а не строку
  char* buffer = NULL;
  size_t inputLen = 0;

  if (context != NULL) {
    inputLen = strlen(str);
    buffer = (char*)malloc(inputLen + 1);
    if (buffer == NULL) {
      setError(context, ERR_NO_MEMORY);
    } else {
      memcpy(buffer, str, inputLen);
      buffer[inputLen] = '\0';
      if (context->inputNum != NULL) {
        free(context->inputNum);
      }
      context->inputNum = buffer;
    }
  }
}

void copyMessage(char* dst, size_t dstSize, const char* src) {
  if (dst != NULL && dstSize > 0) {
    size_t index = 0;
    dst[0] = '\0';
    if (src != NULL){
      for (; index < dstSize - 1 && (dst[index] = src[index]) != '\0'; index++);
      dst[index] = '\0';
    }
  }
}

void clearResult(AppContext* context) {
  if (context != NULL && context->resultNum != NULL) {
    free(context->resultNum);
    context->resultNum = NULL;
  }
}

void swap(AppContext* context) {
  int tempBase = 0;
  char* tempString = NULL;

  if (context != NULL) {
    tempBase = context->inputBase;
    tempString = context->inputNum;
    context->inputBase = context->outputBase;
    context->outputBase = tempBase;
    context->inputNum = context->resultNum;
    context->resultNum = tempString;
    clearError(context);
  }
}

uint32_t parseUnsignedInBase(const char* str, int base, int* overflow) {
  uint32_t value = 0;
  int index = 0;

  if (overflow != NULL)
    *overflow = 0;


  for (; str[index] != '\0'; index++) {
    int digit = charToDigit(str[index]);
    uint64_t nextValue = (uint64_t)value * base + digit;
    if (nextValue > UINT32_MAX) {
      if (overflow != NULL) {
        *overflow = 1;
      }
    }
    value = (uint32_t)nextValue;
  }

  return value;
}

int validateBase(int base) {
  return (base >= MIN_BASE && base <= MAX_BASE);
}

int charToDigit(char symbol) {
  int value = -1;

  if (symbol >= '0' && symbol <= '9') {
    value = symbol - '0';
  } else if (symbol >= 'A' && symbol <= 'Z') {
    value = symbol - 'A' + DECIMAL_BASE;
  } else if (symbol >= 'a' && symbol <= 'z') {
    value = symbol - 'a' + DECIMAL_BASE;
  }
  return value;
}
int validateStringForBase(const char* str, int base) {
  int index = 0;
  int isValid = 1;

  if (str == NULL)
    isValid = 0;

  if (isValid && base == DECIMAL_BASE && (str[0] == '-'))
    index = 1;
  else if (isValid && base != DECIMAL_BASE && (str[0] == '-'))
    isValid = 0;

  for (; isValid && str[index] != '\0'; index++) {
    int digit = charToDigit(str[index]);
    if (digit < 0 || digit >= base) {
      isValid = 0;
    }
  }
  return isValid;
}

uint32_t parseToUint32(const char* str, int base, int* overflow) {
  uint32_t parsedValue = 0;

  if (overflow != NULL)
    *overflow = 0;

  if (base == DECIMAL_BASE) {
    long long value = 0;
    int i = 0;
    int isNegative = 0;

    if (str[0] == '-'){
      isNegative = (str[0] == '-');
      i = 1;
    }

    for (; str[i] != '\0' && !*overflow; i++) {
      int digit = charToDigit(str[i]);
      value = value * DECIMAL_BASE + digit;

      if (!isNegative && value > INT32_MAX)
        *overflow = 1;

      if (isNegative && -value < INT32_MIN)
        *overflow = 1;
    }

    if (!*overflow) {
      if (isNegative)
        value = -value;
      parsedValue = (uint32_t)((int32_t)value);
    }

  } else {
    parsedValue = parseUnsignedInBase(str, base, overflow);
  }

  return parsedValue;
}

void validateAll(AppContext* context) {
  int overflow = 0;

  if (context != NULL) {
    clearError(context);

    if (!validateBase(context->inputBase) || !validateBase(context->outputBase)) {
      setError(context, ERR_INVALID_BASE);
    } else if (context->inputNum == NULL || context->inputNum[0] == '\0') {
      setError(context, ERR_EMPTY_INPUT);
    } else if (!validateStringForBase(context->inputNum, context->inputBase)) {
      setError(context, ERR_INVALID_CHAR);
    }else {
      context->parsedValue = parseToUint32(context->inputNum, context->inputBase, &overflow);
      if (overflow) {
        setError(context, ERR_OVERFLOW);
      }
    }
  }
}

void doInit(AppContext* context) {
  initContext(context);
}

void doInputChanged(AppContext* context, const AppParams* params) {
  if (context != NULL && params != NULL) {
    setInput(context, params->inputNum);
    clearResult(context);
    if (context->errorCode != ERR_NO_MEMORY) {
      validateAll(context);
    }
  }
}

void doConvertClicked(AppContext* context, const AppParams* params) {
  if (context != NULL) {
    if (params != NULL) {
      setInput(context, params->inputNum);
    }

    if (context->errorCode != ERR_NO_MEMORY) {
      validateAll(context);
      if (context->errorCode == STATUS_OK) {
        convert(context);
      } else {
        clearResult(context);
      }
    }
  }
}

void doSwapClicked(AppContext* context) {
  if (context != NULL) {
    swap(context);
    validateAll(context);
  }
}
