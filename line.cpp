
#include "line.h"
#include <math.h>
#include <cstdlib>
#include <cstring>

line::line(const line& orig){
  p1 = orig.p1;
  p2 = orig.p2;
  dx = orig.dx;
  dy = orig.dy;

  xTravel = orig.xTravel;

  fill = new obj::pnt[getNumPoints()];

  memcpy(fill, orig.fill, getNumPoints());
}

line::line(obj::pnt a, obj::pnt b, bool BAmode){
  int Dx = b.x - a.x, Dy = b.y - a.y;
  
  if (abs(Dx) > abs(Dy)){
    xTravel = true;
    
    if (b.x > a.x){
      p1 = a;
      p2 = b;
      
      dx = Dx;
      dy = Dy;
    }
    else{
      p1 = b;
      p2 = a;
      
      dx = -Dx;
      dy = -Dy;
    }
  }
  else{
    xTravel = false;
    
    if (b.y > a.y){
      p1 = a;
      p2 = b;
      
      dx = Dx;
      dy = Dy;
    }
    else{
      p1 = b;
      p2 = a;
      
      dx = -Dx;
      dy = -Dy;
    }
  }
  
  Fill(BAmode);
}

line::~line(){
  delete [] fill;
}

/*
line& operator= (const line& other){
  if(this != &other){
    p1 = orig.p1;
    p2 = orig.p2;
    dx = orig.dx;
    dy = orig.dy;

    xTravel = orig.xTravel;
    memcpy(fill, other.fill, other.getNumPoints());
  }

  return *this;
}
*/


void line::Fill(bool BAmode){
  fill = new obj::pnt[(xTravel?dx:dy)+1];  

  if(dx == 0){
    for(int i = 0; i <= dy; i++){
      fill[i].x = p1.x;
      fill[i].y = p1.y + i;
    }
  }
  else if(dy == 0){
    for(int i = 0; i <= dx; i++){
      fill[i].y = p1.y;
      fill[i].x = p1.x+i;
    }
  }
  else if(dy == dx){
    for(int i = 0; i <= dx; i++){
      fill[i].x = p1.x + i;
      fill[i].y = p1.y + i;
    }
  }
  else if(dy == -dx){
    for(int i = 0; i <= dy; i++){
      fill[i].y = p1.y + i;
      fill[i].x = p1.x - i;
    }
  }
  else if(BAmode){
    //printf("\n\rDraw in BA");
    if(xTravel){
      int currY, p, Dy;
      bool yNeg;

      currY = p1.y;     
 
      //printf("\n\rtest1");

      if(yNeg = (dy < 0)){
        Dy = -dy;
      }
      else Dy = dy;
      //printf("\n\rcY %i, p %i, dx %i, dy %i", currY, p, dx, Dy);
      
      p = 2*Dy - dx;
      
      //printf("\n\rcY %i, p %i, dx %i, dy %i", currY, p, dx, Dy);

      for(int i = 0; i <= dx; i++){
        fill[i].x = p1.x+i;
        fill[i].y = currY;
        if (p>=0) yNeg?currY--:currY++;
        p += 2*Dy - (p<0?0:2*dx);
      }
    }
    else{
      int currX, p, Dx;
      bool xNeg;
      currX = p1.x;

      if(xNeg = (dx < 0))Dx = -dx;
      else Dx = dx;
      
      p = 2*Dx - dy;
      
      for(int i = 0; i <= dy; i++){
        fill[i].y = p1.y+i;
        fill[i].x = currX;
        if (p>=0) xNeg?currX--:currX++;
        p += 2*Dx - (p<0?0:2*dy);
      }
    }
  }
  else{
    if(xTravel){
      const double m = ((double)dy)/dx;
      
      for(int i = 0; i <= dx; i++){
        fill[i].x = p1.x + i;
        fill[i].y = p1.y + (int)(i*m+(dy>0?.5:-.5));
      }
    }
    else{
      const double m = ((double)dx)/dy;
      
      for(int i = 0; i <= dy; i++){
        fill[i].y = p1.y + i;
        fill[i].x = p1.x + (int)(i*m+(dx>0?.5:-.5));
      }
    }
  }
}

