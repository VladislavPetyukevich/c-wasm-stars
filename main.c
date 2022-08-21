#define SCREEN_WIDTH 200
#define SCREEN_HEIGHT 200
#define SCREEN_IMAGE_DATA_LENGTH SCREEN_WIDTH * SCREEN_HEIGHT * 4
#define STARS_COUNT 200
#define FLY_SPEED 66;

static unsigned long int next = 1;

int rand(void) {
  next = next * 1103515245 + 12345;
  return (unsigned int)(next/65536) % 32768;
}

void srand(unsigned int seed) {
  next = seed;
}

int randInRange(int min, int max) {
  return (rand() % (max - min + 1)) + min;
}

int round(double x) {
  if (x < 0.0)
    return (int)(x - 0.5);
  else
   return (int)(x + 0.5);
}

typedef struct {
  float x, y;
} Vector2;

typedef struct {
  float x, y, z;
} Vector3;

unsigned char screenImageData[SCREEN_WIDTH * SCREEN_HEIGHT * 4];

Vector3 stars[STARS_COUNT];

int getScreenWidth () {
  return SCREEN_WIDTH;
}

int getScreenHeight () {
  return SCREEN_HEIGHT;
}

unsigned char* getScreenImageData() {
  return screenImageData;
}

void fillPixel(int x, int y, unsigned char brightness) {
  int pixelPos = (y * SCREEN_WIDTH + x) * 4;
  screenImageData[pixelPos + 0] = brightness;
  screenImageData[pixelPos + 1] = brightness;
  screenImageData[pixelPos + 2] = brightness;
}

void clearScreen() {
  for (int x = 0; x < SCREEN_WIDTH; x++) {
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
      int pixelPos = (y * SCREEN_WIDTH + x) * 4;
      fillPixel(x, y, 0);
      screenImageData[pixelPos + 3] = 255;
    }
  }
}

void drawStar(Vector2* starProjected, int depth) {
  int x = round(starProjected->x);
  int y = round(starProjected->y);
  fillPixel(x, y, depth);

  // fillPixel(x + 1, y + 1, depth);
  // fillPixel(x - 1, y + 1, depth);
  // fillPixel(x + 1, y - 1, depth);
  // fillPixel(x - 1, y - 1, depth);

  fillPixel(x - 1, y, depth);
  fillPixel(x + 1, y, depth);
  fillPixel(x, y - 1, depth);
  fillPixel(x, y + 1, depth);
}

Vector3 createRandVector3() {
  Vector3 vector3 = {
    .x = randInRange(-SCREEN_WIDTH / 2, SCREEN_WIDTH / 2),
    .y = randInRange(-SCREEN_HEIGHT / 2, SCREEN_HEIGHT / 2),
    .z = randInRange(255 / 6, 255),
  };
  return vector3;
}

Vector2 projectVector3(Vector3* vector3) {
  Vector2 vector2 = {
    .x = ((vector3->x * 256) / vector3->z) + SCREEN_WIDTH / 2,
    .y = ((vector3->y * 256) / vector3->z) + SCREEN_HEIGHT / 2,
  };
  return vector2;
}

void initStars() {
  for (int i = 0; i < STARS_COUNT; i++) {
    stars[i] = createRandVector3();
  }
}

unsigned char init(unsigned char *someData) {
  initStars();
  return someData[1];
}

void update (float delta) {
  clearScreen();
  for (int i = 0; i < STARS_COUNT; i++) {
    Vector3* star = &stars[i];
    // star->x += delta * 50;
    // star->y += delta * 50;
    star->z -= delta * FLY_SPEED;
    if (star->z <= 0) {
      *star = createRandVector3();
    }
    Vector2 starProjected = projectVector3(star);
    if (
        (starProjected.x < 1 || starProjected.x > SCREEN_WIDTH - 1) ||
        (starProjected.y < 1 || starProjected.y > SCREEN_HEIGHT - 1)
    ) {
      *star = createRandVector3();
      continue;
    }
    drawStar(&starProjected, 255 - star->z);
  }
}

