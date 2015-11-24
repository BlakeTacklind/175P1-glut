#include <GL/glut.h>
#include "OpenGLhandler.h"
#include "userInterface.h"
#include "screen2d.h"
#include "curve2d.h"
#include <math.h>
#include "MakePixFunc.h"
#include "MakePixOff.h"
// #include "MakeMPixOff.h"
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <list>
using namespace std;

float* OpenGLhandler::PixelBuffer;
// OpenGLhandler::algMode OpenGLhandler::aMode;
int OpenGLhandler::MainWindow;
// OpenGLhandler::drawMode OpenGLhandler::dMode;
// int OpenGLhandler::xMin;
// int OpenGLhandler::yMin;
// int OpenGLhandler::xMax;
// int OpenGLhandler::yMax;
unsigned int OpenGLhandler::width;
unsigned int OpenGLhandler::height;
// pnt3 OpenGLhandler::ambiant;
// pnt3 OpenGLhandler::diffuse;
// pnt3 OpenGLhandler::specular;
  //approximating the “average distance” between the scene and the light source
// float OpenGLhandler::averageLightDistance;
// float OpenGLhandler::ambientIntensity;
// float OpenGLhandler::lightIntensity;
// pnt3 OpenGLhandler::lightPosition;
// unsigned int OpenGLhandler::lightSize;
// OpenGLhandler::lightModel OpenGLhandler::lMode;
// pnt3 OpenGLhandler::mPixTone;
// unsigned int OpenGLhandler::mPixWidth = 3;
// unsigned int OpenGLhandler::mPixHeight = 3;
// OpenGLhandler::pixelModel OpenGLhandler::pMode = Color;
unsigned int OpenGLhandler::resolution;


void OpenGLhandler::initValues(int argc, char** argv){
  
  width = 402;
  height = 402;
  
  PixelBuffer = new float[width * height * 3];

  setResolution(100);

  new screen2d(width, height, new MakePixOff(0,0));
}

void OpenGLhandler::init(int* argc, char** argv)
{
  glutInit(argc, argv);
  glutInitDisplayMode(GLUT_SINGLE);

  glutInitWindowSize(width, height);

  glutInitWindowPosition(100, 100);

  MainWindow = glutCreateWindow("Blake Tacklind - 997051049 - Project 3");
  glClearColor(0, 0, 0, 0);
  glutDisplayFunc(display);

  glutKeyboardFunc(Keystroke);
  glutMouseFunc(mouseClick);
  glutMotionFunc(userInterface::mouseMove);
  glutPassiveMotionFunc(userInterface::mouseMovePassive);
  
  glutMainLoop();

  return;
}

/*
 */
void OpenGLhandler::onClose(void){
  delete [] PixelBuffer;
  curve2d::freeAll();
  screen2d::freeAll();
  userInterface::endUI();
}

void OpenGLhandler::Keystroke(unsigned char key, int x, int y){
  if(!userInterface::isValueMode() && key == 27){
    onClose();
    glutDestroyWindow(MainWindow);
  }
  else userInterface::keypressed(key);
}

// void OpenGLhandler::MakePix(int x, int y){
//   PixelBuffer[(y*width + x) * 3 ]    = 1;
//   PixelBuffer[(y*width + x) * 3 + 1] = 1;
//   PixelBuffer[(y*width + x) * 3 + 2] = 1;
// }

void OpenGLhandler::MakeCPix(int x, int y, pnt3 color){
  PixelBuffer[(y*width + x) * 3 ]    = color.x;
  PixelBuffer[(y*width + x) * 3 + 1] = color.y;
  PixelBuffer[(y*width + x) * 3 + 2] = color.z;
}

/*
void swapBits(int& bits, unsigned int i, unsigned int j){
  if(i==j) return;
  if((1<<i) & bits){
    if((1<<j) & bits){
      return;
    }
    else{
      bits &= ~(1<<i);
      bits |=   1<<j ;
    }
  }
  else{
    if((1<<j) & bits){
      bits &= ~(1<<j);
      bits |=   1<<i ;
    }
    else{
      return;
    }
  }
}

void OpenGLhandler::shuffleBits(int& bits){
  if(bits == 0) return;
  unsigned int numBits = mPixWidth * mPixHeight;
  if(bits == (pow(2, numBits) - 1)) return;

  for(unsigned int i = 0; i < numBits - 1; i++){
    swapBits(bits, i, i+rand()%(numBits-i));

  }
}
void OpenGLhandler::MakeMPix(int x, int y, unsigned int intensity){
  int binOnOff = pow(2, intensity) - 1;
  shuffleBits(binOnOff);
  for(int i = 0; i < mPixWidth * mPixHeight; i++){
    if(binOnOff & 1 << i)
      MakeCPix(x * mPixWidth+ i % mPixWidth, y * mPixHeight + i / mPixWidth, mPixTone); 
  }
}
*/
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
// void OpenGLhandler::bufferObjects(drawMode m, pnt3 view, int x, int y){

  
// }

// void OpenGLhandler::drawLine(line* l){
//   const int dist = l->getNumPoints();
//   for(int i = 0; i < dist; i++){
//     pnt p = l->getPoint(i);
//     MakePix(p.x, p.y);
//   }
  
//   delete l;
// }


void OpenGLhandler::reDraw(){
  glutPostRedisplay();
}
/*
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
*/

void OpenGLhandler::mouseClick(int button, int state, int x, int y){
  if (GLUT_LEFT_BUTTON == button){
    userInterface::leftMouseClick(x, y, GLUT_DOWN==state);
  }
}

void OpenGLhandler::mouseMotion(int x, int y) {
  userInterface::mouseMove(x,y);
}

