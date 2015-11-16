
#ifndef OPENGLHANDLER_H
#define OPENGLHANDLER_H

#include <GL/glut.h>
#include <list>
#include "line.h"
#include "screen.h"
#include "types.h"

using namespace std;

class OpenGLhandler{
  friend class MakePixOff;
  friend class MakeMPixOff;
public:
  enum drawMode {points, lines, fill};
  enum algMode {DDA, BA};
  enum lightModel {Phong, Gouraud};
  enum pixelModel {Color, Mega};

  static void initValues(int argc, char** agrv);
  static void init(int* argc, char** argv);
  static void reDraw();

  //Free space
  static void onClose(void);

  inline static void bufferObjects(){clearBuffer(); screen::bufferAllScreens();};

  static const char* getDrawModeStr();
  static const char* getAlgModeStr();

  inline static drawMode getDrawMode(){return dMode;};

  static void tglDrawMode();
  static void tglAlgMode();
  static void tglLightMode();
  
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


  inline static pnt3 getAmbiant() {return ambiant;};
  inline static void setAmbiant(pnt3 p){ambiant = p;};
  
  inline static pnt3 getDiffuse() {return diffuse;};
  inline static void setDiffuse(pnt3 p){diffuse = p;};
  
  inline static pnt3 getSpecular() {return specular;};
  inline static void setSpecular(pnt3 p){specular = p;};
  
  inline static float getK(){return averageLightDistance;};
  inline static void  setK(float p){averageLightDistance = p;};
  
  inline static float getIa(){return ambientIntensity;};
  inline static float setIa(float i){ambientIntensity = i;};
  
  inline static float getIl(){return lightIntensity;};
  inline static float setIl(float i){lightIntensity = i;};
  
  inline static pnt3 getLpos() {return lightPosition;};
  inline static void setLpos(pnt3 p){lightPosition = p;};
  
  inline static unsigned int getLightSize(){return lightSize;};
  inline static void setLightSize(unsigned int i){lightSize = i;};

  inline static lightModel getLightModel(){return lMode;};
  inline static void setLightModel(lightModel l){lMode = l;};
  
  //clear Pixel Buffer
  static void clearBuffer();
private:
  static void MakePix(int x, int y);
  static void MakeCPix(int x, int y, pnt3 color);
  static void MakeMPix(int x, int y, unsigned int intensity, pnt3 color);
  //line drawing
  static void drawLine(line* l);

  //Add objects to buffer
  static void bufferObjects(drawMode m, pnt3 view, int x, int y);
  
  //Lighting
  static pnt3 ambiant, diffuse, specular;
  //approximating the “average distance” between the scene and the light source
  static float averageLightDistance;
  static float ambientIntensity;
  static float lightIntensity;
  static pnt3 lightPosition;
  static unsigned int lightSize;
  //static pnt3 viewPoint;  //in screen
  
  
  //calculation and draw modes
  static algMode aMode;
  static drawMode dMode;
  static lightModel lMode;
  static pixelModel pMode;

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

#endif
