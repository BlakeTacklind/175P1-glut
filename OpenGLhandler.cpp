#include <GL/glut.h>
#include "OpenGLhandler.h"
#include "userInterface.h"

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

void OpenGLhandler::reDraw(){
  
  glutPostRedisplay();
}

const char* OpenGLhandler::getDrawMode(){
  if (dMode == points) return "Points";
  if (dMode == lines) return "Lines";
  if (dMode == fill) return "Fill";
}

const char* OpenGLhandler::getAlgMode(){
  if (aMode == DDA) return "DDA";
  if (aMode == BA) return "BA";
}

void OpenGLhandler::tglDrawMode(){
       if (dMode == points) dMode = lines;
  else if (dMode == lines ) dMode = points;
  else if (dMode == fill  ) dMode = points;
}

void OpenGLhandler::tglAlgMode(){
       if (aMode == DDA) aMode = BA;
  else if (aMode == BA ) aMode = DDA;
}

void OpenGLhandler::init(int* argc, char** argv)
{
  PixelBuffer = new float[200 * 200 * 3];

  aMode = BA;
  dMode = points;
  xMin = 5;
  yMin = 12;
  xMax = 150;
  yMax = 200;

  //cout<<"test2\n";

  bufferObjects();
  //cout<<"test2.5\n";


  glutInit(argc, argv);
  glutInitDisplayMode(GLUT_SINGLE);

  glutInitWindowSize(200, 200);

  glutInitWindowPosition(100, 100);

  MainWindow = glutCreateWindow("Blake Tacklind - 997051049 - Project 1");
  glClearColor(0, 0, 0, 0);
  glutDisplayFunc(display);
  //cout<<"test3\n";

  glutKeyboardFunc(Keystroke);
  //glutCloseFunc(onClose);
  //cout<<"test4\n";

  userInterface::init();
  //cout<<"test5\n";

  glutMainLoop();

  //cout << "test1\n";
  return;
}

void OpenGLhandler::onClose(void){
  delete [] PixelBuffer;
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

void OpenGLhandler::clearBuffer(){
  for (int i = 0; i < 200*200*3; i++)
    PixelBuffer[i] = 0.0;
}

/*
 * Draw objects depending on draw mode
 */
void OpenGLhandler::bufferObjects(drawMode m){
  clearBuffer();
  obj::clipObjects(xMin, xMax, yMin, yMax);

  //Draw object vertexes
  if (m == points){
    for(int i = 0; i < obj::getNumClippedObjects(); i++){
      obj o = obj::getClippedObject(i);
      
      for(int j = 0; j < o.getNumPoints(); j++){
        obj::pnt p = o.getPoints()[j];
        MakePix(p.x, p.y);
      }
    }
  }
  //Draw object with wireframe
  else if (m == lines){
    for(int i = 0; i < obj::getNumClippedObjects(); i++){
      obj o = obj::getClippedObject(i);
      obj::pnt p1;
      obj::pnt p2 = o.getPoints()[0];
      
      for(int j = 1; j < o.getNumPoints(); j++){
        p1 = p2;
        p2 = o.getPoints()[j];
        
        drawLine(obj::line(p1,p2,aMode==BA));
      }
     
      //close shape (only if more then a line)
      
      if(o.getNumPoints() > 2)
        drawLine(obj::line(o.getPoints()[0], p2,aMode==BA));
      else if (o.getNumPoints() == 1)
        MakePix(p2.x, p2.y);
      //cout << "testl3\n"; 
    }
  }
  //Rasterize objects
  else if (m == fill){
    list<obj::line> lLine;
    
    //iterate through all objects (after clipping)
    for(int i = 0; i < obj::getNumClippedObjects(); i++){
      obj o = obj::getClippedObject(i);
      
      //for all polygons get the non-horizontal lines
      if(o.getNumPoints() > 2){
        
        //get line from first and last point
        obj::line l = obj::line(o.getPoints()[0], o.getPoints()[o.getNumPoints()-1], aMode==BA);
        if (!l.isHorizontal()) lLine.push_front(l);
      
        //get lines from polygon
        for(int j = 1; j < o.getNumPoints(); j++){
          obj::line l = obj::line(o.getPoints()[j], o.getPoints()[j-1], aMode==BA);
          if (!l.isHorizontal()) lLine.push_front(l);
        }
      }
      //draw line object
      else if(o.getNumPoints() == 2){
        drawLine(obj::line(o.getPoints()[0], o.getPoints()[1], aMode==BA));
      }
      //draw pixel object
      else if(o.getNumPoints() == 1){
        MakePix(o.getPoints()[0].x, o.getPoints()[0].y);
      }
    }
    
    //scan through horizontal lines
    for (int i = yMin; i <= yMax; i++){
      list<obj::line> temp(lLine);
      bool draw = false;
      
      //shorten the list to lines that are in this horizontal scan
      shortenList(&temp, i);
      
      //scan each pixel for a line
      for(int j = xMin; j <= xMax; j++){
        bool *Draw = findInList(&temp, j, i);
        
        //if found odd number of lines flip the parity
        if(Draw[0]) draw = ~draw;
        
        //if found a line or are on draw parity make a pixel
        if(draw || Draw[1]) MakePix(j, i);
      }
    }
  }
  
  //cout << "testx\n";
}

/*
 * find if a line is at x location
 */
bool* OpenGLhandler::findInList(list<obj::line>* l, int x, int y){
  bool output1 = false;
  bool output2 = false;
  list<obj::line>::iterator it = l->begin();
  
  for(; it != l->end();){
    int i = 0;
    while((*it).getPoint(i).y != y) i++;
    
    if((*it).getPoint(i).x == x){
      output1 = ~output1;
      l->erase(it);
      output2 = true;
    }
    else it++;
    
  }
  
  bool* out = {output1,output2};
  return out;
}

void OpenGLhandler::shortenList(list<obj::line>* l, int y){
  list<obj::line>::iterator it = l->begin();
  
  for(; it != l->end();){
    int y1 = (*it).getP1().y;
    int y2 = (*it).getP2().y;
    
    if(y1 > y2){
      int temp = y2;
      y2 = y1;
      y1 = temp;
    }
    
    if(y > y2 || y < y1) l->erase(it);
    else it++;
  }
}

void OpenGLhandler::drawLine(obj::line l){
  const int dist = l.getNumPoints();
  for(int i = 0; i <= dist; i++){
    obj::pnt p = l.getPoint(i);
    MakePix(p.x, p.y);
  }
}

void OpenGLhandler::drawLine(const obj::pnt a, const obj::pnt b){
  cout << "testp "<<a.y<<' '<<b.y<<endl;
  const int dx = b.x - a.x;
  const int dy = b.y - a.y;
  cout << "testp "<<dy<<endl;

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
      bool yNeg;
      //const for BA
      if(aMode == BA){
        currY = sp.y;
        if (dx < 0){
          Dx = -dx;
          Dy = -dy;
        }
        else{
          Dx = dx;
          Dy = dy;
        }
        
        if(Dy < 0){
          yNeg = true;
          Dy = -Dy;
        }
        else yNeg = false;

        p = 2 * Dy - Dx;
      }
      //Const for DDA
      else m = ((float)dy)/dx;

      for(int i = 0; i <= Dx; i++){
        if(aMode == DDA){
          MakePix(sp.x+i, sp.y+(int)(i*m+.5));
        }
        else if(aMode == BA){
          if(p>=0) yNeg?currY--:currY++;
          p = p + 2*Dy - (p<0?0:2*Dx);
          MakePix(sp.x+i, currY);
        }
      }
    }
    //Travel in Y direction
    else{
      cout << "testy1\n";
      //setup for algorithms
      const obj::pnt sp = a.y < b.y ? a : b;
      float m;
      int currX, p;
      int Dx, Dy;
      bool xNeg;
      //const for BA
      if(aMode == BA){
        currX = sp.x;
        if (dy < 0){
          Dx = -dx;
          Dy = -dy;
        }
        else{
          Dx = dx;
          Dy = dy;
        }
        
        if(Dx < 0){
          xNeg = true;
          Dx = -Dx;
        }
        else xNeg = false;

        p = 2 * Dx - Dy;

      }
      //Const for DDA
      else m = ((float)dx)/dy;

      cout << "testy2 " << Dy << "\n";
      for(int i = 0; i <= Dy; i++){
        if(aMode == DDA){
          MakePix(sp.x+(int)(i*m+.5), sp.y+i);
        }
        else if(aMode == BA){
          if(p>=0) xNeg?currX--:currX++;
          p = p + 2*Dx -(p<0?0:2*Dy);//
          MakePix(currX, sp.y+i);
        }
      }
      cout << "testy3\n";
    }
  }

  return;
}

