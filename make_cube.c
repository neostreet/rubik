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

static char usage[] =
"usage: make_cube (-debug) in_cube face_color twist outfile\n";

static char couldnt_open[] = "couldn't open %s\n";
static char couldnt_get_status[] = "couldn't get status of %s\n";

int main(int argc,char **argv)
{
  int m;
  int n;
  int o;
  int curr_arg;
  bool bDebug;
  struct stat statbuf;
  off_t bytes_to_read;
  int fhndl;
  int bytes_read;
  int face_color;
  int twist;
  int bytes_to_io;
  char *old_cpt;
  char *new_cpt;

  if ((argc < 5) || (argc > 6)) {
    printf(usage);
    return 1;
  }

  bDebug = false;

  for (curr_arg = 1; curr_arg < argc; curr_arg++) {
    if (!strcmp(argv[curr_arg],"-debug"))
      bDebug = true;
    else
      break;
  }

  if (argc - curr_arg != 4) {
    printf(usage);
    return 2;
  }

  if (!strcmp(argv[curr_arg],"original")) {
    for (m = 0; m < NUM_FACES; m++) {
      for (n = 0; n < FACE_HEIGHT; n++) {
        for (o = 0; o < FACE_WIDTH; o++) {
          old_cube[m][n][o] = m;
        }
      }
    }
  }
  else {
    if (stat(argv[curr_arg],&statbuf) == -1) {
      printf(couldnt_get_status,argv[curr_arg]);
      return 3;
    }

    if (statbuf.st_size != NUM_SQUARES) {
      printf("%s has the wrong size\n",argv[curr_arg]);
      return 4;
    }

    if ((fhndl = open(argv[curr_arg],O_BINARY | O_RDONLY,0)) == -1) {
      printf(couldnt_open,argv[curr_arg]);
      return 5;
    }

    bytes_to_read = statbuf.st_size;

    bytes_read = read(fhndl,old_cube,bytes_to_read);

    if (bytes_read != bytes_to_read) {
      printf("read of %d bytes failed\n",bytes_to_read);
      return 6;
    }

    close(fhndl);
  }

  for (m = 0; m < NUM_FACES; m++) {
    if (!strcmp(argv[curr_arg+1],colors[m]))
      break;
  }

  if (m == NUM_FACES) {
    printf("invalid color: %s\n",argv[curr_arg+1]);
    return 7;
  }

  face_color = m;

  for (m = 0; m < NUM_TWISTS; m++) {
    if (!strcmp(argv[curr_arg+2],twists[m]))
      break;
  }

  if (m == NUM_TWISTS) {
    printf("invalid twist: %s\n",argv[curr_arg+2]);
    return 8;
  }

  twist = m;

  if (twist == TWIST_NO_TWIST)
    ;
  else {
    copy_cube((char *)new_cube,(char *)old_cube);

    old_cpt = (char *)old_cube;
    new_cpt = (char *)new_cube;

    switch(face_color) {
      case FACE_COLOR_RED:
        for (n = 0; n < NUM_MOVES_IN_TWIST; n++) {
          if (twist == TWIST_CLOCKWISE)
            new_cpt[moves[n].to] = old_cpt[moves[n].from];
          else
            new_cpt[moves[n].from] = old_cpt[moves[n].to];
        }

        break;
    }

    copy_cube((char *)old_cube,(char *)new_cube);
  }

  if ((fhndl = open(argv[curr_arg+3],
    O_CREAT | O_EXCL | O_BINARY | O_WRONLY,
    S_IREAD | S_IWRITE)) == -1) {
    printf(couldnt_open,argv[curr_arg+3]);
    return 9;
  }

  bytes_to_io = NUM_SQUARES;

  if (write(fhndl,old_cube,bytes_to_io) != bytes_to_io) {
    printf("write failed\n");
    close(fhndl);
    return 10;
  }

  close(fhndl);

  if (bDebug) {
    for (m = 0; m < NUM_FACES; m++) {
      for (n = 0; n < FACE_HEIGHT; n++) {
        for (o = 0; o < FACE_WIDTH; o++) {
          printf("%d",old_cube[m][n][o]);

          if (o < FACE_WIDTH - 1)
            putchar(' ');
          else
            putchar(0x0a);
        }
      }

      if (m < NUM_FACES - 1)
        putchar(0x0a);
    }
  }

  return 0;
}
