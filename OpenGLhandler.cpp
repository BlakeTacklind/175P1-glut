#include <GL/glut.h>
#include "OpenGLhandler.h"
#include "userInterface.h"
#include "screen.h"
#include "object3D.h"
#include "object3Dsurface.h"

#include <iostream>
#include <list>
using namespace std;

float* OpenGLhandler::PixelBuffer;
OpenGLhandler::algMode OpenGLhandler::aMode;
int OpenGLhandler::MainWindow;
OpenGLhandler::drawMode OpenGLhandler::dMode;
int OpenGLhandler::xMin;
int OpenGLhandler::yMin;
int OpenGLhandler::xMax;
int OpenGLhandler::yMax;
unsigned int OpenGLhandler::width;
unsigned int OpenGLhandler::height;
pnt3 OpenGLhandler::ambiant;
pnt3 OpenGLhandler::diffuse;
pnt3 OpenGLhandler::specular;
  //approximating the “average distance” between the scene and the light source
float OpenGLhandler::averageLightDistance;
float OpenGLhandler::ambientIntensity;
float OpenGLhandler::lightIntensity;
pnt3 OpenGLhandler::lightPosition;
unsigned int OpenGLhandler::lightSize;
OpenGLhandler::lightModel OpenGLhandler::lMode;

void OpenGLhandler::initValues(int argc, char** argv){
  
  width = 402;
  height = 402;
  
  PixelBuffer = new float[width * height * 3];

  const pnt3 red   = {1,0,0};
  const pnt3 green = {0,1,0};
  const pnt3 blue  = {0,0,1};

  const pnt3 pnt10 = {1,1,2};

  setAmbiant(red);
  setDiffuse(green);
  setSpecular(blue);
  setK(1.0);
  setIa(.5);
  setIl(2);
  setLpos(pnt10);
  setLightSize(3);
  // setLightModel(Gouraud);
  setLightModel(Phong);
  
  tglDrawMode();
  tglDrawMode();

  pnt3 iso = {0.612375, 0.612375, -0.50000};
  
  new screen(width/2, height/2, 0      , 0       , unitX, 10, MakeCPix);
  new screen(width/2, height/2, width/2, height/2, unitY, 10, MakeCPix);
  new screen(width/2, height/2, 0      , height/2, -unitZ, 10, MakeCPix);
  new screen(width/2, height/2, width/2, 0       , iso  , 10, MakeCPix);

}

void OpenGLhandler::init(int* argc, char** argv)
{
  glutInit(argc, argv);
  glutInitDisplayMode(GLUT_SINGLE);

  glutInitWindowSize(width, height);

  glutInitWindowPosition(100, 100);

  MainWindow = glutCreateWindow("Blake Tacklind - 997051049 - Project 2");
  glClearColor(0, 0, 0, 0);
  glutDisplayFunc(display);

  glutKeyboardFunc(Keystroke);
  
  glutMainLoop();

  return;
}

/*
 */
void OpenGLhandler::onClose(void){
  delete [] PixelBuffer;
  object3D::freeAll();
  screen::freeAll();
  userInterface::endUI();
}

void OpenGLhandler::Keystroke(unsigned char key, int x, int y){
  if(!userInterface::isValueMode() && key == 27){
    onClose();
    glutDestroyWindow(MainWindow);
  }
  else userInterface::keypressed(key);
}

void OpenGLhandler::MakePix(int x, int y){
  PixelBuffer[(y*width + x) * 3 ]    = 1;
  PixelBuffer[(y*width + x) * 3 + 1] = 1;
  PixelBuffer[(y*width + x) * 3 + 2] = 1;
}

void OpenGLhandler::MakeCPix(int x, int y, pnt3 color){
  PixelBuffer[(y*width + x) * 3 ]    = color.x;
  PixelBuffer[(y*width + x) * 3 + 1] = color.y;
  PixelBuffer[(y*width + x) * 3 + 2] = color.z;
}

void OpenGLhandler::MakeMPix(int x, int y, int intensity){
  int binOnOff = pow(2, intensity) - 1;
  for(int i = 0; i < 3*3; i++){
    if(biOnOff & 1 << i) MakeCPix(x*3+i%3, y*3+i/3, {1,1,1}); 
  }
}

void OpenGLhandler::display(){
  glClear(GL_COLOR_BUFFER_BIT);
  glLoadIdentity();

  glDrawPixels(width, height, GL_RGB, GL_FLOAT, PixelBuffer);

  glEnd();
  glFlush();

  return;
}

void OpenGLhandler::clearBuffer(){
  for (int i = 0; i < width*height*3; i++)
    PixelBuffer[i] = 0.0;
}

/*
 * Draw objects depending on draw mode
 */
void OpenGLhandler::bufferObjects(drawMode m, pnt3 view, int x, int y){
  clearBuffer();
  list<surface*> surfaces;
  
  //create list of surfaces
  for (int i = 0; i < object3Dsurface::getNumObjects(); i++){
    object3Dsurface* obj = object3Dsurface::getObject(i);
    for (int j = 0; j < obj->getNumSurface(); j++){
      surfaces.push_back(obj->getSurface(j));
    }
  }
  
  //reduce list of surfaces
  for(list<surface*>::iterator it = surfaces.begin(); it != surfaces.end(); ){
    if((*it)->getNormal() * view >= 0)
      surfaces.erase(it);
    else
      it++;
  }
  
  //sort list of surfaces
  
  
  //get intensity of points
  
  
  
  //fill points to screen

  //Draw object vertexes
  if (m == points){
    
  }
  //Draw object with wireframe
  else if (m == lines){
    
  }
  //Rasterize objects
  else if (m == fill){
    
  }
  
}

void OpenGLhandler::drawLine(line* l){
  const int dist = l->getNumPoints();
  for(int i = 0; i < dist; i++){
    pnt p = l->getPoint(i);
    MakePix(p.x, p.y);
  }
  
  delete l;
}


void OpenGLhandler::reDraw(){
  glutPostRedisplay();
}

const char* OpenGLhandler::getDrawModeStr(){
  if (dMode == points) return "Points";
  if (dMode == lines ) return "Lines";
  if (dMode == fill  ) return "Fill";
}

const char* OpenGLhandler::getAlgModeStr(){
  if (aMode == DDA) return "DDA";
  if (aMode == BA ) return "BA";
}

void OpenGLhandler::tglDrawMode(){
       if (dMode == points) dMode = lines;
  else if (dMode == lines ) dMode = fill;
  else if (dMode == fill  ) dMode = points;
}

void OpenGLhandler::tglAlgMode(){
       if (aMode == DDA) aMode = BA;
  else if (aMode == BA ) aMode = DDA;
}

void OpenGLhandler::tglLightMode(){
       if (lMode == Phong  ) lMode = Gouraud;
  else if (lMode == Gouraud) lMode = Phong;
}
