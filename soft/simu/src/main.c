#include "field/field.h"
#include "pathfinder/pathfinder.h"
#include "position/position.h"
#include "screen/screen.h"
#include "config.h"

int main (void)
{
  fieldMapInit ();

  fieldMapDesine ();
  screenPause ();

  posSetXmm (1500-750);
  posSetYmm (1000);
  posSetAdeg (90);

  pathfinderInit ();
//  pathfinderMapDump ();
  pathfinderMapDump2 ();
  pathfinderMapDesine ();
  pathfinderGotoXYmm (2500, 1600);
  screenPause ();

  posSetXmm (2500);
  posSetYmm (1600);
  posSetAdeg (90);

  pathfinderInit ();
//  pathfinderMapDump ();
//  pathfinderMapDump2 ();
  pathfinderMapDesine ();
  pathfinderGotoXYmm (2500, 400);
  screenPause ();

  screenStop ();

  return EXIT_SUCCESS;
}
