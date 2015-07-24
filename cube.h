#define FACE_COLOR_RED      0
#define FACE_COLOR_ORANGE   1
#define FACE_COLOR_YELLOW   2
#define FACE_COLOR_GREEN    3
#define FACE_COLOR_BLUE     4
#define FACE_COLOR_PURPLE   5
#define NUM_FACES           6

#define FACE_WIDTH 3
#define FACE_HEIGHT 3

#define NUM_SQUARES (NUM_FACES * FACE_WIDTH * FACE_HEIGHT)

#define TWIST_NO_TWIST          0
#define TWIST_CLOCKWISE         1
#define TWIST_COUNTERCLOCKWISE  2
#define NUM_TWISTS              3

#ifdef DEFINE_GLOBALS_HERE
char old_cube[NUM_FACES][FACE_HEIGHT][FACE_WIDTH];
char new_cube[NUM_FACES][FACE_HEIGHT][FACE_WIDTH];

char *colors[] = {
  "red",
  "orange",
  "yellow",
  "green",
  "blue",
  "purple"
};

char *twists[] = {
  "no_twist",
  "clockwise",
  "counterclockwise"
};
#else
extern char old_cube[NUM_FACES][FACE_HEIGHT][FACE_WIDTH];
extern char new_cube[NUM_FACES][FACE_HEIGHT][FACE_WIDTH];
extern char *colors[];
extern char *twists[];
#endif

void copy_cube(char *dest,char *source);
