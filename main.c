#include "robot.h"

int main(int argc, char **argv) {
  NcursesInit();

  Localize(argc, argv);

  NcursesQuit();

  return 0;
}
