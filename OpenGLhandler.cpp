#include <GL/glut.h>
#include "OpenGLhandler.h"
#include "userInterface.h"
#include "screen.h"
#include "object3D.h"

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

void OpenGLhandler::initValues(int argc, char** argv){
  
  width = 400;
  height = 400;
  
  PixelBuffer = new float[width * height * 3];

  pnt3 iso = {0.612375, 0.612375, -0.50000};
  
  new screen(width/2, height/2, 0      , 0       , unitX, MakePix);
  new screen(width/2, height/2, width/2, height/2, unitY, MakePix);
  new screen(width/2, height/2, 0      , height/2, unitZ, MakePix);
  new screen(width/2, height/2, width/2, 0       , iso  , MakePix);

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
void OpenGLhandler::bufferObjects(drawMode m){
  clearBuffer();
  object2D::clipObjects(xMin, xMax, yMin, yMax);

  //Draw object vertexes
  if (m == points){
    
    for(list<object2D*>::iterator itr = object2D::getClippedList().begin(); itr != object2D::getClippedList().end(); itr++){
      for(int j = 0; j < (*itr)->getNumPoints(); j++){
        pnt* p = &((*itr)->getPoints()[j]);
        MakePix(p->x, p->y);
      }
    }
  }
  //Draw object with wireframe
  else if (m == lines){
    for(list<object2D*>::iterator itr = object2D::getClippedList().begin(); itr != object2D::getClippedList().end(); itr++){
      pnt p1;
      pnt p2 = (*itr)->getPoints()[0];
      
      for(int j = 1; j < (*itr)->getNumPoints(); j++){
        p1 = p2;
        p2 = (*itr)->getPoints()[j];
        drawLine(new line(p1,p2,aMode==BA));
      }
     
      //close shape (only if more then a line)
      if((*itr)->getNumPoints() > 2){
        drawLine(new line((*itr)->getPoints()[0], p2,aMode==BA));
      }
      //draw object if its a line
      else if ((*itr)->getNumPoints() == 1)
        MakePix(p2.x, p2.y);
    }
  }
  //Rasterize objects
  else if (m == fill){
    //iterate through all objects (after clipping)
    for(list<object2D*>::iterator itr = object2D::getClippedList().begin(); itr != object2D::getClippedList().end(); itr++){
      (*itr)->fill(MakePix,  aMode==BA);
    }
  }
  
  object2D::cleanClippedObects();
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

const char* OpenGLhandler::getDrawMode(){
  if (dMode == points) return "Points";
  if (dMode == lines ) return "Lines";
  if (dMode == fill  ) return "Fill";
}

const char* OpenGLhandler::getAlgMode(){
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
