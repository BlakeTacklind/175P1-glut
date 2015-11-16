
#include "line.h"
#include <math.h>
#include <cstdlib>
#include <cstring>
#include <iostream>

using namespace std;

/*
 * create a line and fill its points using the correct algorithm
 */
line::line(pnt a, pnt b, bool BAmode){
  int Dx = b.x - a.x, Dy = b.y - a.y;
  
  //cout<<"test l1"<<endl;
  //determine which direction to travel
  //and which point is the first point
  if (abs(Dx) > abs(Dy)){
    xTravel = true;
    
    //set direction so always left to right
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
    
    //set direction so always left to right
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
  //cout<<"test l6"<<endl;
  
  Fill(BAmode);
  //cout<<"test le"<<endl;
}

//Free line space
line::~line(){
  delete [] fill;
}

//get the points on the line with the proper algorithm
void line::Fill(bool BAmode){
  fill = new pnt[getNumPoints()];  

  //special case vertical
  if(dx == 0){
    for(int i = 0; i <= dy; i++){
      fill[i].x = p1.x;
      fill[i].y = p1.y + i;
    }
  }
  //special case horizontal
  else if(dy == 0){
    for(int i = 0; i <= dx; i++){
      fill[i].y = p1.y;
      fill[i].x = p1.x+i;
    }
  }
  //special case m=1
  else if(dy == dx){
    for(int i = 0; i <= dx; i++){
      fill[i].x = p1.x + i;
      fill[i].y = p1.y + i;
    }
  }
  //special case m=-1
  else if(dy == -dx){
    for(int i = 0; i <= dy; i++){
      fill[i].y = p1.y + i;
      fill[i].x = p1.x - i;
    }
  }
  //run algorithms
  
  //Run BA algorithm
  else if(BAmode){
    //cout<<"test l9"<<endl;
    
    //Line travels in x direction
    if(xTravel){
      int currY, p, Dy;
      bool yNeg;

      currY = p1.y;     

      if(yNeg = (dy < 0))Dy = -dy;
      else Dy = dy;
      
      p = 2*Dy - dx;

      for(int i = 0; i <= dx; i++){
        fill[i].x = p1.x+i;
        fill[i].y = currY;
        if (p>=0) yNeg?currY--:currY++;
        p += 2*Dy - (p<0?0:2*dx);
      }
    }
    //Line travels in y direction
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
  //Run DDA algorithm
  else{
    if(xTravel){
      const double m = ((double)dy)/dx;
      
      for(int i = 0; i <= dx; i++){
        fill[i].x = p1.x + i;
        //round in direction of slope
        fill[i].y = p1.y + (int)(i*m+(dy>0?.5:-.5));
      }
    }
    else{
      const double m = ((double)dx)/dy;
      
      for(int i = 0; i <= dy; i++){
        fill[i].y = p1.y + i;
        //round in direction of slope
        fill[i].x = p1.x + (int)(i*m+(dx>0?.5:-.5));
      }
    }
  }
}


int* line::findFrontBackAtLine(int y){
  if(p1.y > y && p2.y > y) return nullptr;
  if(p1.y < y && p2.y < y) return nullptr;

  int* I = new int[2];
  bool first = true;

  for(int i = 0; i < getNumPoints(); i++){
    if(first){
      if(getPoint(i).y == y){
        I[0] = i;
        first = false;
        
        if(i == getNumPoints()-1){
          I[1] = i;
          return I;
        }
      }
    }
    else{
      if(getPoint(i).y != y){
        I[1] = i-1;
        return I;
      }

      if(i == getNumPoints()-1){
        I[1] = i;
        return I;
      }
    }
  }

  delete I;
  return nullptr;
}
