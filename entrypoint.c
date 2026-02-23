#include "entrypoint.h"
#include "logic.h"

void doOperation(AppContext* context, Operation operation, AppParams* params){
  if (context != NULL) {
    switch (operation) {
      case INIT:
        doInit(context);
        break;
      case INPUT_CHANGED:
        doInputChanged(context, params);
        break;
      case CONVERT_CLICKED:
        doConvertClicked(context, params);
        break;
      case SWAP_CLICKED:
        doSwapClicked(context);
        break;
      default:
        break;
    }
  }
}
