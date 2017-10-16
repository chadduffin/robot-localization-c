#ifndef __ROBOT__
#define __ROBOT__

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NORTH 0x4E4E
#define SOUTH 0x5353
#define EAST 0x4545
#define WEST 0x5757
#define NORTHEAST 0x454E
#define NORTHWEST 0x574E
#define SOUTHEAST 0x4553
#define SOUTHWEST 0x5753

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
struct Matrix* CreateErrorMatrix(long double err);
struct Matrix* CreateCollisionMatrix(char *filename);
struct Matrix* CreateDirectionMatrix(struct Matrix *map);
struct Matrix* CreateTransitivityMatrix(struct Matrix *direction);
struct Matrix* CreateJointPredictionMatrix(struct Matrix *collision);
unsigned int DirectionDifference(Direction a, Direction b);
unsigned int DirectionAdjacentOpen(Direction dir);
unsigned int DirectionFromChar(char *observation);
void FillObservationMatrix(struct Matrix *observation, struct Matrix *transitivity, struct Matrix *error, unsigned int dir);
void DirectionComponents(Direction dir, int *x, int *y);
void Localize(int argc, char **argv);

/* matrix.c */
struct Matrix* MatrixCreate(unsigned int row, unsigned int col);
struct Matrix* MatrixMultiply(struct Matrix *a, struct Matrix *b);
long double MatrixGet(struct Matrix *matrix, unsigned int row, unsigned int col);
void MatrixSet(struct Matrix *matrix, unsigned int row, unsigned int col, long double value);
void MatrixOutput(struct Matrix *matrix);
void MatrixOutputInt(struct Matrix *matrix);
void MatrixDestroy(struct Matrix **matrix);

#endif /* __ROBOT__ */
