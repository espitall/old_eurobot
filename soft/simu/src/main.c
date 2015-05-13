#include "field/field.h"
#include "pathfinder/pathfinder.h"
#include "position/position.h"
#include "screen/screen.h"
#include "config.h"

int main (void)
{
  //  fieldMapInit ();
  fieldMapInit2 ();

  fieldMapDesine ();
  screenPause ();

  posSetXmm (1500-700);
  posSetYmm (1000);
  posSetAdeg (90);

  pathfinderInit ();
//  pathfinderMapDump ();
//  pathfinderMapDump2 ();
  pathfinderMapDesine ();
  pathfinderGotoXYmm (2400, 1700);
  screenPause ();

  posSetXmm (2400);
  posSetYmm (1700);
  posSetAdeg (90);

  pathfinderInit ();
//  pathfinderMapDump ();
//  pathfinderMapDump2 ();
  pathfinderMapDesine ();
  pathfinderGotoXYmm (2200, 200);
  screenPause ();

  posSetXmm (2200);
  posSetYmm (200);
  posSetAdeg (0);

  pathfinderInit ();
//  pathfinderMapDump ();
//  pathfinderMapDump2 ();
  pathfinderMapDesine ();
  pathfinderGotoXYmm (700, 200);
  screenPause ();

  posSetXmm (700);
  posSetYmm (200);
  posSetAdeg (0);

  pathfinderInit ();
//  pathfinderMapDump ();
//  pathfinderMapDump2 ();
  pathfinderMapDesine ();
  pathfinderGotoXYmm (500, 1700);
  screenPause ();

  screenStop ();

  return EXIT_SUCCESS;
}
