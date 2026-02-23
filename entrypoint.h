#ifndef ENTRYPOINT_H
#define ENTRYPOINT_H

#include "appcontext.h"

typedef enum{
    INIT,
    INPUT_CHANGED,
    CONVERT_CLICKED,
    SWAP_CLICKED
} Operation;

typedef struct{
    const char* inputNum;
} AppParams;

void doOperation(AppContext* context, Operation operation, AppParams* params);

#endif // ENTRYPOINT_H
