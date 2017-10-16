#include "robot.h"

void NcursesInit(void) {
  initscr();
  raw();
  noecho();
  start_color();

  init_color(32, 100, 500, 100);
  init_pair(1, COLOR_BLACK, 32);
}

void NcursesQuit(void) {
  endwin();
}

void NcursesRender(struct Matrix *collision, struct Matrix *result) {
  long double max;
  unsigned int i, n;

  max = -1.0;
  n = -1;

  for (i = 0; i < result->len; i += 1) {
    if (MatrixGet(result, i, 0) > max) {
      max = MatrixGet(result, i, 0);
      n = i;
    }
  }

  for (i = 0; i < result->len; i += 1) {
    if (MatrixGet(collision, i/collision->col, i%collision->col) == 1) {
      mvaddch(i/collision->col, i%collision->col, '#');
    } else {
      if (MatrixGet(result, i, 0) >= max*0.1) {
        attron(COLOR_PAIR(1));
        mvaddch(i/collision->col, i%collision->col, '.');
        attroff(COLOR_PAIR(1));
      } else {
        mvaddch(i/collision->col, i%collision->col, '.');
      }
    }
  }

  refresh();

  getch();
}
