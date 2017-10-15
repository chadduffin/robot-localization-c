#include "robot.h"

struct Matrix* MatrixCreate(unsigned int row, unsigned int col) {
  struct Matrix *matrix = (struct Matrix*)malloc(sizeof(struct Matrix));

  matrix->row = row;
  matrix->col = col;
  matrix->len = row*col;

  matrix->self = (long double*)malloc(sizeof(long double)*(matrix->len));

  memset((void*)(matrix->self), 0, sizeof(long double)*(row*col));

  return matrix;
}

struct Matrix* MatrixMultiply(struct Matrix *a, struct Matrix *b) {
  struct Matrix *c = MatrixCreate(a->row, b->col);

  unsigned int i, j, k;

  for (i = 0; i < a->col; i += 1) {
    for (j = 0; j < b->row; j += 1) {
      for (k = 0; k < a->row; k += 1) {
        *(c->self+j+i*c->col) = *(a->self+k+i*a->col) + *(b->self+j+k*b->col);
      }
    }
  }

  return c;
}

void MatrixOutput(struct Matrix *matrix) {
  unsigned int i, j;

  for (i = 0; i < matrix->len; i += 1) {
    printf((i%matrix->col == matrix->col-1) ? "%.12Le\n" : "%.12Le  ", *(matrix->self+i));
  }
}

void MatrixDestroy(struct Matrix **matrix) {
  free(*matrix);
}
