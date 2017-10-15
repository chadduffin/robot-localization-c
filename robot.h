#ifndef __ROBOT__
#define __ROBOT__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum Direction {
  EE = 1,
  NE = 2,
  NN = 4,
  NW = 8,
  WW = 16,
  SW = 32,
  SS = 64,
  SE = 128,

  AL = EE | NE | NN | NW | WW | SW | SS | SE
} Direction;

struct Matrix {
  long double *self;
  unsigned int row, col, len;
};

/* robot.c */
struct Matrix* CreateDirectionMatrix(struct Matrix *map);
struct Matrix* CreateTransitivityMatrix(struct Matrix *direction);
unsigned int DirectionDifference(Direction a, Direction b);
void DirectionComponents(Direction dir, int *x, int *y);

/* matrix.c */
struct Matrix* LoadMap(char *filename);
struct Matrix* MatrixCreate(unsigned int row, unsigned int col);
struct Matrix* MatrixMultiply(struct Matrix *a, struct Matrix *b);
long double MatrixGet(struct Matrix *matrix, unsigned int row, unsigned int col);
void MatrixSet(struct Matrix *matrix, unsigned int row, unsigned int col, long double value);
void MatrixOutput(struct Matrix *matrix);
void MatrixDestroy(struct Matrix **matrix);

#endif /* __ROBOT__ */
