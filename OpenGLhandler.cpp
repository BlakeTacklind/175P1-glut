#include <GL/glut.h>
#include "OpenGLhandler.h"
#include "userInterface.h"

#include <iostream>
using namespace std;

float* OpenGLhandler::PixelBuffer;
OpenGLhandler::algMode OpenGLhandler::aMode;
int OpenGLhandler::MainWindow;

void OpenGLhandler::init(int* argc, char** argv)
{
  PixelBuffer = new float[200 * 200 * 3];

  //MakePix(10,10);
  //MakePix(1,1);
  
  aMode = DDA;

  //bufferObjects(points);
  bufferObjects(lines);

  glutInit(argc, argv);
  glutInitDisplayMode(GLUT_SINGLE);

  glutInitWindowSize(200, 200);

  glutInitWindowPosition(100, 100);

  MainWindow = glutCreateWindow("Blake Tacklind - 997051049 - Project 1");
  glClearColor(0, 0, 0, 0);
  glutDisplayFunc(display);

  glutKeyboardFunc(Keystroke);
  //glutCloseFunc(onClose);

  glutMainLoop();

  cout << "test1\n";
  return;
}

void OpenGLhandler::onClose(void){
  delete PixelBuffer;
  obj::freeAll();
  userInterface::endUI();
  //cout << "closing\n";
}

void OpenGLhandler::Keystroke(unsigned char key, int x, int y){
  //glutReshapeWindow(400,400);

  //glutSwapBuffers();
  //MakePix(0,0);

  //glutPostRedisplay();
  if(key == 27){
    onClose();
    glutDestroyWindow(MainWindow);
  }
  else userInterface::keypressed(key);

  //cout << key << endl;
  //cout << x << ' ' << y << endl;
}

void OpenGLhandler::MakePix(int x, int y){
  PixelBuffer[(y*200 + x) * 3 ]    = 1;
  PixelBuffer[(y*200 + x) * 3 + 1] = 1;
  PixelBuffer[(y*200 + x) * 3 + 2] = 1;
}

void OpenGLhandler::display()
{
  glClear(GL_COLOR_BUFFER_BIT);
  glLoadIdentity();

  glDrawPixels(200, 200, GL_RGB, GL_FLOAT, PixelBuffer);

  glEnd();
  glFlush();

  return;
}

/*
 * Draw objects depending on draw mode
 */
void OpenGLhandler::bufferObjects(drawMode m){
  //Draw object vertexes
  if (m == points){
    for(int i = 0; i < obj::getNumObjects(); i++){
      obj o = obj::getObject(i);
      for(int j = 0; j < o.getNumPoints(); j++){
        obj::pnt p = o.getPoints()[j];
        MakePix(p.x, p.y);
      }
    }
  }
  //Draw object with wireframe
  else if (m == lines){
    for(int i = 0; i < obj::getNumObjects(); i++){
      obj o = obj::getObject(i);
      obj::pnt p1;
      obj::pnt p2 = o.getPoints()[0];
      for(int j = 1; j < o.getNumPoints(); j++){
        p1 = p2;
        p2 = o.getPoints()[j];
        drawLine(p1,p2);
      }
      
      drawLine(o.getPoints()[0], p2);
    }
  }
  else if (m == fill){
    
  }

}

void OpenGLhandler::drawLine(const obj::pnt a, const obj::pnt b){
  const int dx = b.x - a.x;
  const int dy = b.y - a.y;

  /*
   * Special cases
   */
  //Horizontal line
  if (dx == 0){
    for(int i = min(b.y, a.y); i <= max(a.y, b.y); i++){
      MakePix(a.x, i);
    }
  }
  //Vertical line
  else if (dy == 0){
    for(int i = min(a.x, b.x); i <= max(b.x, a.x); i++){
      MakePix(i, a.y);
    }
  }
  //m=1 line
  else if (dy == dx){
    const int sx = min(a.x, b.x);
    const int sy = min(a.y, b.y);
    
    for(int i = 0; i < abs(dx); i++){
      MakePix(sx+i, sy+i);
    }
  }
  //m=-1 line
  else if (dy == -dx){
    const int sx = min(a.x, b.x);
    const int sy = max(a.y, b.y);

    for(int i = 0; i < abs(dx); i++){
      MakePix(sx+i, sy-i);
    }
  }
  //run algorithms
  else{
    //Travel in x direstion
    if(abs(dx) > abs(dy)){
      //setup for algorithms
      const obj::pnt sp = a.x < b.x ? a : b;
      float m;
      int currY, p;
      int Dx, Dy;
      //const for BA
      if(aMode == BA){
        currY = sp.y;
        p = 2 * dy - dx;
        if (dx < 0){
          Dx = -dx;
          Dy = -dy;
        }
        else{
          Dx = dx;
          Dy = dy;
        }
      }
      //Const for DDA
      else m = ((float)dy)/dx;

      for(int i = 0; i <= abs(dx); i++){
        if(aMode == DDA){
          MakePix(sp.x+i, sp.y+(int)(i*m+.5));
        }
        else if(aMode == BA){
          p = p + 2*Dy - (p<0?0:2*Dx);
          if(p>=0) currY++;
          MakePix(sp.x+i, currY);
        }
      }
    }
    //Travel in Y direction
    else{
      //setup for algorithms
      const obj::pnt sp = a.y < b.y ? a : b;
      float m;
      int currX, p;
      int Dx, Dy;
      //const for BA
      if(aMode == BA){
        currX = sp.x;
        p = 2 * dx - dy;
        if (dy < 0){
          Dx = -dx;
          Dy = -dy;
        }
        else{
          Dx = dx;
          Dy = dy;
        }
      }
      //Const for DDA
      else m = ((float)dx)/dy;

      for(int i = 0; i <= abs(dy); i++){
        if(aMode == DDA){
          MakePix(sp.x+(int)(i*m+.5), sp.y+i);
        }
        else if(aMode == BA){
          p = p + 2*Dx -(p<0?0:2*Dy);//(Dx>0?1:-1)*
          if(p>=0)currX++;// (Dx>0)?currX++:currX--;
          MakePix(currX, sp.y+i);
        }
      }
    }
  }

  return;
}

