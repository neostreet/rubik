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

void copy_cube(char *dest,char *source)
{
  int n;

  for (n = 0; n < NUM_SQUARES; n++)
    dest[n] = source[n];
}
