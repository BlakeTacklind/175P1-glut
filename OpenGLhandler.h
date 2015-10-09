#include <GL/glut.h>
#include "obj.h"
//#include <string>
using namespace std;

class OpenGLhandler{
public:
  enum drawMode {points, lines, fill};
  enum algMode {DDA, BA};
  static void init(int* argc, char** argv);
  static void reDraw();

  inline static void bufferObjects(){bufferObjects(dMode);};

  static const char* getDrawMode();
  static const char* getAlgMode();

  static void tglDrawMode();
  static void tglAlgMode();

private:
  static void clearBuffer();
  static void MakePix(int x, int y);
  static void drawLine(obj::pnt a, obj::pnt b);
  static void onClose(void);
  static void bufferObjects(drawMode m);

  static algMode aMode;
  static drawMode dMode;
  static int MainWindow;

  static void display();
  static float *PixelBuffer;
  static void Keystroke(unsigned char key, int x, int y);

};

