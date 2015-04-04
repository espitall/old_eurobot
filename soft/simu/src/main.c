#include <stdlib.h>
#include "field/field.h"

int main(void)
{
  fieldMapInit();
  fieldMatrixInit();

  //fieldMapAffiche();
  fieldMapDesine();

  //fieldMatrixAffiche();
  fieldMatrixDesine();

  return EXIT_SUCCESS;
}
