#include "robot.h"

int main(int argc, char **argv) {
  struct Matrix *matrix = LoadMap("input"), *direction = CreateDirectionMatrix(matrix);

  MatrixOutput(matrix);

  printf("\n");

  MatrixOutput(direction);

  MatrixDestroy(&matrix);
  MatrixDestroy(&direction);

  return 0;
}
