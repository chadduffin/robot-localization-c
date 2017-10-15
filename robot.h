#ifndef __ROBOT__
#define __ROBOT__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Matrix {
  long double *self;
  unsigned int row, col, len;
};

struct Matrix* MatrixCreate(unsigned int row, unsigned int col);
struct Matrix* MatrixMultiply(struct Matrix *a, struct Matrix *b);
void MatrixOutput(struct Matrix *matrix);
void MatrixDestroy(struct Matrix **matrix);

#endif /* __ROBOT__ */
