#include <stdio.h>

#include "robot.h"

int main(int argc, char **argv) {
  struct Matrix *matrix = MatrixCreate(4, 3);

  MatrixOutput(matrix);

  MatrixDestroy(&matrix);

  return 0;
}
