#include <stdlib.h>
#include "field/field.h"

int main(void)
{
  fieldMatrixInit();
  fieldMatrixWithObstaclesInit();

  //fieldMatrixAffiche();
  fieldMatrixDesine();

  //fieldMatrixWithObstaclesAffiche();
  fieldMatrixWithObstaclesDesine();

  return EXIT_SUCCESS;
}
