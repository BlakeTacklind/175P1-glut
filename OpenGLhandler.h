#include <GL/glut.h>
#include "obj.h"
#include <list>
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
  //clear Pixel Buffer
  static void clearBuffer();

  static void MakePix(int x, int y);
  //line drawing
  static void drawLine(obj::line l);
  //Old line drawing
  static void drawLine(obj::pnt a, obj::pnt b);
  //Free space
  static void onClose(void);
  //Add objects to buffer
  static void bufferObjects(drawMode m);
  
  //functions to for rasterization
  static void findInList(list<obj::line>* l, int x, int y, bool* out);
  static void shortenList(list<obj::line>* l, int y);

  //calculation and draw modes
  static algMode aMode;
  static drawMode dMode;

  //Main window handle
  static int MainWindow;

  //viewport values
  static int xMin, yMin, xMax, yMax;

  static void display();
  
  //Pixel Buffer handler
  static float *PixelBuffer;
  
  //Keystroke callback
  static void Keystroke(unsigned char key, int x, int y);

};

