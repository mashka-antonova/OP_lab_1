#include "convert.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "logic.h"
//разобраться с приведение типов удалить если лишнее


void swapChars(char* a, char* b) {
  char temp = *a;
  *a = *b;
  *b = temp;
}

void reverseString(char* str, int len) {
  for (int left = 0, right = len - 1; left < right; left++, right--)
    swapChars(&str[left], &str[right]);
}

char digitToChar(int digit) {
  char c;
  if (digit < DECIMAL_BASE)
    c = (char)('0' + digit);
  else
    c = (char)('A' + (digit - DECIMAL_BASE));
  return c;
}

char* formatUnsignedBase(uint32_t value, int base) {
  char* result = malloc(BUFFER_SIZE);
  int len = 0;
  if (result != NULL) {
    do {
      result[len++] = digitToChar((int)value % base);
      value /= base;
    } while (value > 0U);

    reverseString(result, len);
    result[len] = '\0';
  }

  return result;
}

char* formatSignedDecimal(uint32_t value) {
  int32_t signedValue = (int32_t)value;
  uint32_t absValue = value;
  char* result = NULL;

  if (signedValue < 0)
    absValue = (uint32_t)(-(signedValue + 1)) + 1U;

  result = formatUnsignedBase(absValue, DECIMAL_BASE);

  if (result != NULL && signedValue < 0) {
    size_t len = strlen(result);
    memmove(result + 1, result, len + 1);
    result[0] = '-';
  }

  return result;
}


void convert(AppContext* context) {
  char* converted = NULL;

  if (context != NULL && context->errorCode == STATUS_OK) {
    clearResult(context);

    if (context->outputBase == DECIMAL_BASE) {
      converted = formatSignedDecimal(context->parsedValue);
    } else {
      converted = formatUnsignedBase(context->parsedValue, context->outputBase);
    }

    if (converted == NULL) {
      setError(context, ERR_NO_MEMORY);
    } else {
      context->resultNum = converted;
    }
  }
}
