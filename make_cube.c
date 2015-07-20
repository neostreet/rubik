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

static char outfile[] = "make_cube.out";

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
  "clockwise",
  "counterclockwise"
};

static char cube[NUM_FACES][FACE_HEIGHT][FACE_WIDTH];

int main(int argc,char **argv)
{
  int m;
  int n;
  int o;
  int fhndl;
  int bytes_to_io;

  if ((fhndl = open(outfile,
    O_CREAT | O_EXCL | O_BINARY | O_WRONLY,
    S_IREAD | S_IWRITE)) == -1) {
    printf(couldnt_open,outfile);
    return 1;
  }

  for (m = 0; m < NUM_FACES; m++) {
    for (n = 0; n < FACE_HEIGHT; n++) {
      for (o = 0; o < FACE_WIDTH; o++) {
        cube[m][n][o] = m;
      }
    }
  }

  bytes_to_io = NUM_SQUARES;

  if (write(fhndl,cube,bytes_to_io) != bytes_to_io) {
    printf("write failed\n");
    close(fhndl);
    return 2;
  }

  close(fhndl);

  return 0;
}
