#include "field/field.h"
#include "pathfinder/pathfinder.h"
#include "position/position.h"
#include "screen/screen.h"
#include "config.h"

int main (void)
{
  fieldMapInit ();

//  fieldMapDesine ();
//  screenPause ();

  pathfinderInit ();
//  pathfinderMapDump ();
  pathfinderMapDesine ();
  pathfinderGotoXYmm (posGetXmm () + 100, posGetYmm ());
  screenPause ();

  screenStop ();

  return EXIT_SUCCESS;
}
