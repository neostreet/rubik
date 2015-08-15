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
#define DEFINE_GLOBALS_HERE
#include "cube.h"

static char usage[] = "usage: gen_moves infile1 infile2\n";
static char couldnt_open[] = "couldn't open %s\n";
static char couldnt_get_status[] = "couldn't get status of %s\n";

int main(int argc,char **argv)
{
  int m;
  int n;
  struct stat statbuf[2];
  int bytes_to_io[2];
  int bytes_read[2];
  char *buf[2];
  int fhndl[2];
  int num_moves;

  if (argc != 3) {
    printf(usage);
    return 1;
  }

  if (!strcmp(argv[1],argv[2])) {
    printf("infile1 must differ from infile2\n");
    return 2;
  }

  for (n = 0; n < 2; n++) {
    if (stat(argv[1+n],&statbuf[n]) == -1) {
      printf(couldnt_get_status,argv[1+n]);
      return 3;
    }

    bytes_to_io[n] = (int)statbuf[n].st_size;

    if (bytes_to_io[n] != NUM_SQUARES) {
      printf("bytes_to_io[%d] != %d\n",n,NUM_SQUARES);
      return 4;
    }

    if ((buf[n] = (char *)malloc(bytes_to_io[n])) == NULL) {
      printf("malloc of %d bytes failed\n",bytes_to_io);
      return 5;
    }

    if ((fhndl[n] = open(argv[1+n],O_BINARY | O_RDONLY,0)) == -1) {
      printf(couldnt_open,argv[1+n]);
      free(buf[n]);
      return 6;
    }

    bytes_read[n] = read(fhndl[n],buf[n],bytes_to_io[n]);

    if (bytes_read[n] != bytes_to_io[n]) {
      printf("read of %d bytes failed\n",bytes_to_io[n]);
      free(buf[n]);
      close(fhndl[n]);
      return 7;
    }
  }

  for (n = 0; n < 2; n++)
    close(fhndl[n]);

  num_moves = 0;

  for (n = 0; n < NUM_SQUARES; n++) {
    if (buf[0][n] != buf[1][n]) {
      num_moves++;

      for (m = 0; m < NUM_SQUARES; m++) {
        if (buf[0][n] == buf[1][m])
          break;
      }

      printf("%d %d\n",n,m);
    }
  }

  for (n = 0; n < 2; n++)
    free(buf[n]);

  printf("num_moves = %d\n",num_moves);

  return 0;
}
