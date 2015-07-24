#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#ifdef WIN32
#include <io.h>
#else
#include <unistd.h>
#endif
#include "cube.h"

static char usage[] = "usage: make_cube face_color twist outfile\n";

static char couldnt_open[] = "couldn't open %s\n";
static char couldnt_get_status[] = "couldn't get status of %s\n";

int main(int argc,char **argv)
{
  int m;
  int n;
  int o;
  int face_color;
  int twist;
  int fhndl;
  int bytes_to_io;

  if (argc != 4) {
    printf(usage);
    return 1;
  }

  for (m = 0; m < NUM_FACES; m++) {
    for (n = 0; n < FACE_HEIGHT; n++) {
      for (o = 0; o < FACE_WIDTH; o++) {
        old_cube[m][n][o] = m;
      }
    }
  }

  for (m = 0; m < NUM_FACES; m++) {
    if (!strcmp(argv[1],colors[m]))
      break;
  }

  if (m == NUM_FACES) {
    printf("invalid color: %s\n",argv[1]);
    return 2;
  }

  face_color = m;

  for (m = 0; m < NUM_TWISTS; m++) {
    if (!strcmp(argv[2],twists[m]))
      break;
  }

  if (m == NUM_TWISTS) {
    printf("invalid twist: %s\n",argv[2]);
    return 3;
  }

  twist = m;

  if (twist == TWIST_NO_TWIST)
    ;
  else {
    copy_cube((char *)new_cube,(char *)old_cube);

    if (twist == TWIST_CLOCKWISE) {
      new_cube[face_color][0][0] = old_cube[face_color][2][0];
      new_cube[face_color][0][1] = old_cube[face_color][1][0];
      new_cube[face_color][0][2] = old_cube[face_color][0][0];
      new_cube[face_color][1][0] = old_cube[face_color][2][1];
      new_cube[face_color][1][2] = old_cube[face_color][0][1];
      new_cube[face_color][2][0] = old_cube[face_color][2][2];
      new_cube[face_color][2][1] = old_cube[face_color][1][2];
      new_cube[face_color][2][2] = old_cube[face_color][0][2];
    }
    else {
      new_cube[face_color][0][0] = old_cube[face_color][0][2];
      new_cube[face_color][0][1] = old_cube[face_color][1][2];
      new_cube[face_color][0][2] = old_cube[face_color][2][2];
      new_cube[face_color][1][0] = old_cube[face_color][0][1];
      new_cube[face_color][1][2] = old_cube[face_color][2][1];
      new_cube[face_color][2][0] = old_cube[face_color][0][0];
      new_cube[face_color][2][1] = old_cube[face_color][1][0];
      new_cube[face_color][2][2] = old_cube[face_color][2][0];
    }

    switch(face_color) {
      case FACE_COLOR_RED:
        if (twist == TWIST_CLOCKWISE) {
          new_cube[FACE_COLOR_ORANGE][0][0] = old_cube[FACE_COLOR_BLUE][2][0];
          new_cube[FACE_COLOR_ORANGE][1][0] = old_cube[FACE_COLOR_BLUE][2][1];
          new_cube[FACE_COLOR_ORANGE][2][0] = old_cube[FACE_COLOR_BLUE][2][2];

          new_cube[FACE_COLOR_GREEN][0][2] = old_cube[FACE_COLOR_PURPLE][0][0];
          new_cube[FACE_COLOR_GREEN][1][2] = old_cube[FACE_COLOR_PURPLE][0][1];
          new_cube[FACE_COLOR_GREEN][2][2] = old_cube[FACE_COLOR_PURPLE][0][2];

          new_cube[FACE_COLOR_BLUE][2][0] = old_cube[FACE_COLOR_GREEN][0][2];
          new_cube[FACE_COLOR_BLUE][2][1] = old_cube[FACE_COLOR_GREEN][1][2];
          new_cube[FACE_COLOR_BLUE][2][2] = old_cube[FACE_COLOR_GREEN][2][2];

          new_cube[FACE_COLOR_PURPLE][0][0] = old_cube[FACE_COLOR_ORANGE][0][0];
          new_cube[FACE_COLOR_PURPLE][0][1] = old_cube[FACE_COLOR_ORANGE][0][1];
          new_cube[FACE_COLOR_PURPLE][0][2] = old_cube[FACE_COLOR_ORANGE][0][2];
        }
        else {
          new_cube[FACE_COLOR_BLUE][2][0] = old_cube[FACE_COLOR_ORANGE][0][0];
          new_cube[FACE_COLOR_BLUE][2][1] = old_cube[FACE_COLOR_ORANGE][1][0];
          new_cube[FACE_COLOR_BLUE][2][2] = old_cube[FACE_COLOR_ORANGE][2][0];

          new_cube[FACE_COLOR_PURPLE][0][0] = old_cube[FACE_COLOR_GREEN][0][2];
          new_cube[FACE_COLOR_PURPLE][0][1] = old_cube[FACE_COLOR_GREEN][1][2];
          new_cube[FACE_COLOR_PURPLE][0][2] = old_cube[FACE_COLOR_GREEN][2][2];

          new_cube[FACE_COLOR_GREEN][0][2] = old_cube[FACE_COLOR_BLUE][2][0];
          new_cube[FACE_COLOR_GREEN][1][2] = old_cube[FACE_COLOR_BLUE][2][1];
          new_cube[FACE_COLOR_GREEN][2][2] = old_cube[FACE_COLOR_BLUE][2][2];

          new_cube[FACE_COLOR_ORANGE][0][0] = old_cube[FACE_COLOR_PURPLE][0][0];
          new_cube[FACE_COLOR_ORANGE][0][1] = old_cube[FACE_COLOR_PURPLE][0][1];
          new_cube[FACE_COLOR_ORANGE][0][2] = old_cube[FACE_COLOR_PURPLE][0][2];
        }

        break;
    }

    copy_cube((char *)old_cube,(char *)new_cube);
  }

  if ((fhndl = open(argv[3],
    O_CREAT | O_EXCL | O_BINARY | O_WRONLY,
    S_IREAD | S_IWRITE)) == -1) {
    printf(couldnt_open,argv[3]);
    return 4;
  }

  bytes_to_io = NUM_SQUARES;

  if (write(fhndl,old_cube,bytes_to_io) != bytes_to_io) {
    printf("write failed\n");
    close(fhndl);
    return 5;
  }

  close(fhndl);

  return 0;
}
