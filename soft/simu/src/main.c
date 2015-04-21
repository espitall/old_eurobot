#include "field/field.h"
#include "pathfinder/pathfinder.h"
#include "screen/screen.h"
#include "config.h"

int main (void)
{
  fieldMapInit ();

  fieldMapDesine ();
  screenPause ();

  pathfinderInit ();
  pathfinderMapDesine ();
//  pathfinderGotoXYmm (2400, 500);
  screenPause ();

  screenStop ();

  return EXIT_SUCCESS;
}
