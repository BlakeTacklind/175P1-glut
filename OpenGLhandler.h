#include <GL/glut.h>
#include "object2D.h"
#include <list>
#include "line.h"
#include "screen.h"

using namespace std;

class OpenGLhandler{
public:
  enum drawMode {points, lines, fill};
  enum algMode {DDA, BA};

  static void initValues(int argc, char** agrv);
  static void init(int* argc, char** argv);
  static void reDraw();

  //Free space
  static void onClose(void);

  inline static void bufferObjects(){clearBuffer(); screen::bufferAllScreens();};

  static const char* getDrawMode();
  static const char* getAlgMode();

  static void tglDrawMode();
  static void tglAlgMode();
  
  inline static int getYmin(){return yMin;};
  inline static int getYmax(){return yMax;};
  inline static int getXmin(){return xMin;};
  inline static int getXmax(){return xMax;};
  
  inline static int setYmin(int i){yMin = i;};
  inline static int setYmax(int i){yMax = i;};
  inline static int setXmin(int i){xMin = i;};
  inline static int setXmax(int i){xMax = i;};
  
  inline static int getScreenWidth (){return width; };
  inline static int getScreenHeight(){return height;};

  //clear Pixel Buffer
  static void clearBuffer();

  pnt3 getAmbiant()      {return ambiant;};
  void setAmbiant(pnt3 p){ambiant = p;   };
  
  pnt3 getDiffuse()      {return diffuse;};
  void setDiffuse(pnt3 p){diffuse = p;   };
  
  pnt3 getSpecular()      {return specular;};
  void setSpecular(pnt3 p){specular = p;   };
  
  float getK(){return averageLightDistance;};
  void  setK(float p){averageLightDistance = p;};
  
private:
  static void MakePix(int x, int y);
  static void MakeCPix(int x, int y, pnt3 color);
  //line drawing
  static void drawLine(line* l);

  //Add objects to buffer
  static void bufferObjects(drawMode m);
  
  //Lighting
  pnt3 ambiant, diffuse, specular;
  float averageLightDistance;
  
  
  //calculation and draw modes
  static algMode aMode;
  static drawMode dMode;

  //Main window handle
  static int MainWindow;
  //Window sizes
  static unsigned int width;
  static unsigned int height;

  //viewport values
  static int xMin, yMin, xMax, yMax;

  static void display();
  
  //Pixel Buffer handler
  static float *PixelBuffer;
  
  //Keystroke callback
  static void Keystroke(unsigned char key, int x, int y);

};

