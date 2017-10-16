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

  unsigned int i, j, k, icol, jicol;

  for (i = 0; i < a->row; i += 1) {
    icol = i*a->col;

    for (j = 0; j < b->col; j += 1) {
      jicol = j+i*b->col;

      for (k = 0; k < a->col; k += 1) {
        *(c->self+jicol) += (*(a->self+k+icol))*(*(b->self+j+k*b->col));
      }
    }
  }

  return c;
}

long double MatrixGet(struct Matrix *matrix, unsigned int row, unsigned int col) {
  return *(matrix->self+col+row*matrix->col);
}

void MatrixSet(struct Matrix *matrix, unsigned int row, unsigned int col, long double value) {
  *(matrix->self+col+row*matrix->col) = value;
}

void MatrixOutput(struct Matrix *matrix) {
  unsigned int i;

  for (i = 0; i < matrix->len; i += 1) {
    printf((i%matrix->col == matrix->col-1) ? "%.2Le\n" : "%.2Le  ", *(matrix->self+i));
  }
}

void MatrixDestroy(struct Matrix **matrix) {
  free(*matrix);
  
  *matrix = NULL;
}
