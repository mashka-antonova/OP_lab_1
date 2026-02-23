#include "memory.h"
#include <stdlib.h>

void freeContext(AppContext* context){
  if (context != NULL) {
    if (context->inputNum != NULL) {
      free(context->inputNum);
      context->inputNum = NULL;
    }
    if (context->resultNum != NULL) {
      free(context->resultNum);
      context->resultNum = NULL;
    }
  }
}
