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

struct move {
  char from;
  char to;
};

#define NUM_MOVES_IN_TWIST 20

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

struct move moves[NUM_MOVES_IN_TWIST] = {
  { 0, 2},
  { 1, 5},
  { 2, 8},
  { 3, 1},
  { 5, 7},
  { 6, 0},
  { 7, 3},
  { 8, 6},
  { 9, 45},
  { 12, 46},
  { 15, 47},
  { 29, 42},
  { 32, 43},
  { 35, 44},
  { 42, 9},
  { 43, 12},
  { 44, 15},
  { 45, 29},
  { 46, 32},
  { 47, 35},
};
#else
extern char old_cube[NUM_FACES][FACE_HEIGHT][FACE_WIDTH];
extern char new_cube[NUM_FACES][FACE_HEIGHT][FACE_WIDTH];
extern char *colors[];
extern char *twists[];
extern struct move moves[NUM_MOVES_IN_TWIST];
#endif

void copy_cube(char *dest,char *source);
