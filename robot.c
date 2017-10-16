#include "robot.h"

struct Matrix* CreateErrorMatrix(long double err) {
  unsigned int i;
  struct Matrix *error = MatrixCreate(9, 1);

  for (i = 0; i < 9; i += 1) {
    MatrixSet(error, i, 0, (pow(err, i)*pow(1-err, 8-i)));
  }

  return error;
}

struct Matrix* CreateCollisionMatrix(char *filename) {
  struct Matrix *matrix;
  FILE *file = fopen(filename, "r");

  char c;
  unsigned int i, row, col;

  fscanf(file, "%u %u", &row, &col);

  matrix = MatrixCreate(row, col);

  for (i = 0; i < row*col; i += 1) {
    while ((c = fgetc(file)) == '\n');

    MatrixSet(matrix, i/col, i%col, (c == '1') ? 1.0 : 0.0);
  }

  return matrix;
}

struct Matrix* CreateDirectionMatrix(struct Matrix *map) {
  int x, y, col, row;
  unsigned int i, j, val;
  struct Matrix *direction = MatrixCreate(map->row, map->col);

  for (i = 0; i < map->len; i += 1) {
    val = AL;

    if (MatrixGet(map, i/map->col, i%map->col) == 1.0) {
      MatrixSet(direction, i/map->col, i%map->col, AL);
      continue;
    }

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
  int x, y, row, col;
  long double open;
  unsigned int i, j;
  struct Matrix *transitivity = MatrixCreate(direction->len, direction->len);

  for (i = 0; i < direction->len; i += 1) {
    open = 1.0/DirectionAdjacentOpen(MatrixGet(direction, i/direction->col, i%direction->col));

    for (j = EE; j < AL; j = j << 1) {
      if (((unsigned int)MatrixGet(direction, i/direction->col, i%direction->col)) & j) { continue; }
       
      DirectionComponents(j, &x, &y);

      row = i/direction->col+y;
      col = i%direction->col+x;

      if ((row >= 0) && (row < direction->row) &&
          (col >= 0) && (col < direction->col)) {
        MatrixSet(transitivity, col+row*direction->col, i, open);
      }
    }
  }

  return transitivity;
}

struct Matrix* CreateJointPredictionMatrix(struct Matrix *collision) {
  long double probability;
  unsigned int i, n = 0;
  struct Matrix *joint_prediction = MatrixCreate(collision->len, 1);

  for (i = 0; i < collision->len; i += 1) {
    if (MatrixGet(collision, i/collision->col, i%collision->col) == 0.0) {
      n += 1;
    }
  }

  probability = 1.0/n;

  for (i = 0; i < collision->len; i += 1) {
    if (MatrixGet(collision, i/collision->col, i%collision->col) == 0.0) {
      MatrixSet(joint_prediction, i, 0, probability);
    }
  }

  return joint_prediction;
}

unsigned int DirectionDifference(unsigned int a, unsigned int b) {
  return (((a & EE) != (b & EE))+((a & NE) != (b & NE))+((a & NN) != (b & NN))+((a & NW) != (b & NW))+
          ((a & WW) != (b & WW))+((a & SW) != (b & SW))+((a & SS) != (b & SS))+((a & SE) != (b & SE)));
}

unsigned int DirectionAdjacentOpen(unsigned int dir) {
  return (!(dir & EE)+!(dir & NE)+!(dir & NN)+!(dir & NW)+!(dir & WW)+!(dir & SW)+!(dir & SS)+!(dir & SE));
}

unsigned int DirectionFromChar(char *observation) {
  unsigned int i, dir = 0;

  for (i = 0; i < 16; i += 2) {
    if (*(observation+i) == 0) { break; }

    switch (*((unsigned short*)(observation+i))) {
      case EAST:
        dir = dir | EE;
        break;
      case NORTHEAST:
        dir = dir | NE;
        break;
      case NORTH:
        dir = dir | NN;
        break;
      case NORTHWEST:
        dir = dir | NW;
        break;
      case WEST:
        dir = dir | WW;
        break;
      case SOUTHWEST:
        dir = dir | SW;
        break;
      case SOUTH:
        dir = dir | SS;
        break;
      case SOUTHEAST:
        dir = dir | SE;
        break;
    }
  }

  return dir;
}

void FillObservationMatrix(struct Matrix *observation, struct Matrix *direction, struct Matrix *error, unsigned int dir) {
  unsigned int i, tdir, diff;

  for (i = 0; i < observation->row; i += 1) {
    tdir = MatrixGet(direction, i/direction->col, i%direction->col);

    if (tdir == AL) {
      MatrixSet(observation, i, i, 0.0);
    } else {
      diff = DirectionDifference(tdir, dir);
      MatrixSet(observation, i, i, MatrixGet(error, diff, 0));
    }
  }
}

void DirectionComponents(unsigned int dir, int *x, int *y) {
  *x = ((dir & (NE | EE | SE)) ? 1 : 0)-((dir & (NW | WW | SW)) ? 1 : 0);
  *y = ((dir & (SE | SS | SW)) ? 1 : 0)-((dir & (NE | NN | NW)) ? 1 : 0);
}

void Localize(int argc, char **argv) {
  if (argc != 4) {
    printf("usage: ./robot <error> <collision-map-filename> <observation-filename>\n");
    exit(1);
  }

  char o[16];
  unsigned int i, n, dir;
  struct Matrix *error = CreateErrorMatrix(atof(argv[1])),
                *collision = CreateCollisionMatrix(argv[2]),
                *direction = CreateDirectionMatrix(collision),
                *transitivity = CreateTransitivityMatrix(direction),
                *joint_prediction = CreateJointPredictionMatrix(collision),

                *observation = MatrixCreate(collision->len, collision->len),
                *result;
  FILE *file = fopen(argv[3], "r");

  fscanf(file, "%i", &n);

  for (i = 0; i < n; i += 1) {
    fscanf(file, "%s", o);

    dir = DirectionFromChar(o);

    result = MatrixMultiply(transitivity, joint_prediction);

    MatrixDestroy(&joint_prediction);

    FillObservationMatrix(observation, direction, error, dir);

    joint_prediction = MatrixMultiply(observation, result);

    NcursesRender(collision, joint_prediction);

    MatrixDestroy(&result);
  }

  MatrixDestroy(&joint_prediction);
  MatrixDestroy(&transitivity);
  MatrixDestroy(&direction);
  MatrixDestroy(&collision);
  MatrixDestroy(&error);
}
