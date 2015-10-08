#include <GL/glut.h>
#include "obj.h"

class OpenGLhandler{
public:
  enum drawMode {points, lines, fill};
  enum algMode {DDA, BA};
  static void init(int* argc, char** argv);
  static void bufferObjects(drawMode m);

private:
  static void MakePix(int x, int y);
  static void drawLine(obj::pnt a, obj::pnt b);
  static void onClose(void);

  static algMode aMode;

  static void display();
  static float *PixelBuffer;
  static void Keystroke(unsigned char key, int x, int y);

};

