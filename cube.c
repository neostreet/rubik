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
#define O_BINARY 0
#endif
#include "cube.h"

static char usage[] = "usage: cube in_cube face_color twist\n";

static char couldnt_open[] = "couldn't open %s\n";
static char couldnt_get_status[] = "couldn't get status of %s\n";

static char *colors[] = {
  "red",
  "orange",
  "yellow",
  "green",
  "blue",
  "purple"
};

static char *twists[] = {
  "no_twist",
  "clockwise",
  "counterclockwise"
};

static char old_cube[NUM_FACES][FACE_HEIGHT][FACE_WIDTH];
static char new_cube[NUM_FACES][FACE_HEIGHT][FACE_WIDTH];

void copy_cube(char *dest,char *source);

int main(int argc,char **argv)
{
  int m;
  int n;
  int o;
  struct stat statbuf;
  off_t bytes_to_read;
  int fhndl;
  int bytes_read;
  int face_color;
  int twist;

  if (argc != 4) {
    printf(usage);
    return 1;
  }

  if (stat(argv[1],&statbuf) == -1) {
    printf(couldnt_get_status,argv[1]);
    return 2;
  }

  if (statbuf.st_size != NUM_SQUARES) {
    printf("%s has the wrong size\n",argv[1]);
    return 3;
  }

  if ((fhndl = open(argv[1],O_BINARY | O_RDONLY,0)) == -1) {
    printf(couldnt_open,argv[1]);
    return 4;
  }

  bytes_to_read = statbuf.st_size;

  bytes_read = read(fhndl,old_cube,bytes_to_read);

  if (bytes_read != bytes_to_read) {
    printf("read of %d bytes failed\n",bytes_to_read);
    return 5;
  }

  for (m = 0; m < NUM_FACES; m++) {
    if (!strcmp(argv[2],colors[m]))
      break;
  }

  if (m == NUM_FACES) {
    printf("invalid color: %s\n",argv[2]);
    return 6;
  }

  face_color = m;

  for (m = 0; m < NUM_TWISTS; m++) {
    if (!strcmp(argv[3],twists[m]))
      break;
  }

  if (m == NUM_TWISTS) {
    printf("invalid twist: %s\n",argv[3]);
    return 7;
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

          new_cube[FACE_COLOR_BLUE][2][0] = old_cube[FACE_COLOR_GREEN][2][2];
          new_cube[FACE_COLOR_BLUE][2][1] = old_cube[FACE_COLOR_GREEN][0][2];
          new_cube[FACE_COLOR_BLUE][2][2] = old_cube[FACE_COLOR_GREEN][1][2];

          new_cube[FACE_COLOR_PURPLE][0][0] = old_cube[FACE_COLOR_ORANGE][0][0];
          new_cube[FACE_COLOR_PURPLE][0][1] = old_cube[FACE_COLOR_ORANGE][0][1];
          new_cube[FACE_COLOR_PURPLE][0][2] = old_cube[FACE_COLOR_ORANGE][0][2];
        }
        else {
          new_cube[FACE_COLOR_ORANGE][0][2] = old_cube[FACE_COLOR_PURPLE][0][0];
          new_cube[FACE_COLOR_ORANGE][1][2] = old_cube[FACE_COLOR_PURPLE][0][1];
          new_cube[FACE_COLOR_ORANGE][2][2] = old_cube[FACE_COLOR_PURPLE][0][2];
        }

        break;
    }

    copy_cube((char *)old_cube,(char *)new_cube);
  }

  printf("<!DOCTYPE html>\n");
  printf("<html>\n");
  printf("<head>\n");
  printf("  <title>cube</title>\n");
  printf("</head>\n");
  printf("<body>\n\n");

  for (n = 0; n < FACE_HEIGHT; n++) {
    if (n > 0)
      printf("<br>\n");

    for (m = 0; m < NUM_FACES; m++) {
      if (m > 0)
        printf("&nbsp; &nbsp; &nbsp; &nbsp; &nbsp\n");

      for (o = 0; o < FACE_WIDTH; o++)
        printf("<img src=\"%s.bmp\">\n",colors[old_cube[m][n][o]]);
    }
  }

  printf("\n</body>\n");
  printf("</html>\n");

  return 0;
}

void copy_cube(char *dest,char *source)
{
  int n;

  for (n = 0; n < NUM_SQUARES; n++)
    dest[n] = source[n];
}
