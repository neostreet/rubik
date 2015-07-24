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

static char usage[] = "usage: make_html in_cube outfile\n";

static char couldnt_open[] = "couldn't open %s\n";
static char couldnt_get_status[] = "couldn't get status of %s\n";

int main(int argc,char **argv)
{
  int m;
  int n;
  int o;
  struct stat statbuf;
  off_t bytes_to_read;
  int fhndl;
  int bytes_read;
  FILE *fptr;

  if (argc != 3) {
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

  close(fhndl);

  if ((fptr = fopen(argv[2],"w")) == NULL) {
    printf(couldnt_open,argv[2]);
    return 6;
  }

  fprintf(fptr,"<!DOCTYPE html>\n");
  fprintf(fptr,"<html>\n");
  fprintf(fptr,"<head>\n");
  fprintf(fptr,"  <title>cube</title>\n");
  fprintf(fptr,"</head>\n");
  fprintf(fptr,"<body>\n\n");

  for (n = 0; n < FACE_HEIGHT; n++) {
    if (n > 0)
      fprintf(fptr,"<br>\n");

    for (m = 0; m < NUM_FACES; m++) {
      if (m > 0)
        fprintf(fptr,"&nbsp; &nbsp; &nbsp; &nbsp; &nbsp\n");

      for (o = 0; o < FACE_WIDTH; o++)
        fprintf(fptr,"<img src=\"%s.bmp\">\n",colors[old_cube[m][n][o]]);
    }
  }

  fprintf(fptr,"\n</body>\n");
  fprintf(fptr,"</html>\n");

  fclose(fptr);

  return 0;
}
