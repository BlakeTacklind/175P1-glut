#include <GL/glut.h>

class OpenGLhandler{
public:
  static void init(int* argc, char** argv);
  static void MakePix(int x, int y);

private:
  static void display();
  static float *PixelBuffer;
  static void Keystroke(unsigned char key, int x, int y);
};

