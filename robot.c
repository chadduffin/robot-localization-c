#include "robot.h"

struct Matrix* CreateDirectionMatrix(struct Matrix *map) {
  int x, y, col, row;
  unsigned int i, j, val;
  struct Matrix *direction = MatrixCreate(map->row, map->col);

  for (i = 0; i < map->len; i += 1) {
    val = AL;

    for (j = EE; j < AL; j = j << 1) {
      DirectionComponents(j, &x, &y);

      row = i/map->col+y;
      col = i%map->col+x;

      if ((row >= 0) && (row < map->row) &&
          (col >= 0) && (col < map->col) &&
          (MatrixGet(map, row, col) == 0.0)) {
        val = val ^ j;
      }
    }

    MatrixSet(direction, i/map->col, i%map->col, val);
  }

  return direction;
}

struct Matrix* CreateTransitivityMatrix(struct Matrix *direction) {
  struct Matrix *transitivity = MatrixCreate(direction->row, direction->col);

  return transitivity;
}

unsigned int DirectionDifference(Direction a, Direction b) {
  return (((a & NN) == (b & EE))+((a & NE) == (b & NE))+((a & NN) == (b & NN))+((a & NW) == (b & NW))+
          ((a & WW) == (b & WW))+((a & SW) == (b & SW))+((a & SS) == (b & SS))+((a & SE) == (b & SE)));
}

void DirectionComponents(Direction dir, int *x, int *y) {
  *x = ((dir & (NE | EE | SE)) ? 1 : 0)-((dir & (NW | WW | SW)) ? 1 : 0);
  *y = ((dir & (SE | SS | SW)) ? 1 : 0)-((dir & (NE | NN | NW)) ? 1 : 0);
}
