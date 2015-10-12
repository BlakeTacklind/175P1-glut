#include "obj.h"
#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>
#include <math.h>
//#include <curses.h>
#include <list>

#define PI 3.14159265

using namespace std;

unsigned int obj::nObjects;
obj* obj::objectList;
char* obj::storedFileName;
unsigned int obj::nClippedObjects;
obj* obj::clippedObjects;

void obj::clipObjects(int xmin, int xmax, int ymin, int ymax){
  clippedObjects = new obj[nObjects];
  nClippedObjects = 0;

  for(int i = 0; i < nObjects; i++){
    obj o = objectList[i].clip(xmin, xmax, ymin, ymax);
    if(o.getNumPoints()) clippedObjects[nClippedObjects++] = o;
    //cout << "testc " << o.getNumPoints() << endl;
  }
  
  //cout<<"testcn "<<clippedObjects[0].getNumPoints()<<endl;
}

obj::line::line(pnt a, pnt b, bool BAmode){
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

obj::line::~line(){
  delete [] fill;
}

void obj::line::Fill(bool BAmode){
  fill = new pnt[(xTravel?dx:dy)+1];  

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

void obj::freeAll(){
  for(int i=0; i < obj::nObjects; i++){
    free(obj::getObject(i).getPoints());
  }

  free(obj::objectList);
}

obj::obj(unsigned int numPoints, pnt* points){
  nPoints = numPoints;
  pointList = points;
}

void obj::load(char* filename){
  storedFileName = filename;
  
  ifstream file(filename, ios::in);
  
  if(file.is_open()){
    string line;
    
    /*
     * Get the first line of the file
     * It should be number of objects.
     * If bad input close file and spit out error.
     */
    if(getline(file, line)){
      int num = atoi(line.c_str());
      
      if (num < 0){
        cout << "negative objects?!?!\n";
        file.close();
        return;
      }
      else if (num == 0){
        cout << "bad input or zero objects\n";  
        file.close();
        return;
      }
      
      nObjects = num;
      objectList = (obj*)malloc(sizeof(obj) * nObjects);
    }
    else{
      cout << "Empty file!\n";
      file.close();
      return;
    }
    
    /*
     * iterate through the number of objects
     */
    
    for(int i = 0; i < nObjects; i++){
      int num;
      if(getline(file, line)){
        num = atoi(line.c_str());

        if (num < 0){
          cout << "negative points?!?!\n";
          file.close();
          return;
        }
        else if (num == 0){
          cout << "bad input or zero points\n";  
          file.close();
          return;
        }
      }
      else{
        cout << "Missing Object definition\n";
        file.close();
        return;
      }
      
      /*
       * Iterate through the number of points
       */
      pnt* points = (pnt*)malloc(sizeof(pnt) * num);
      for (int j = 0; j < num; j++){
        if(getline(file, line)){
          int del = line.find_first_of(' ');
          if (del < 1){
            cout << "Bad point delimiter\n";
            file.close();
            return;
          }
          
          int x = atoi(line.substr(0, del).c_str());
          int y = atoi(line.substr(del+1).c_str());
          points[j].x = x;
          points[j].y = y;
          
          
        }
        else{
          cout << "Missing Point definition\n";
          file.close();
          return;
        }
      }
      
      //Build Object
      objectList[i] = obj(num, points);
    }
    
    file.close();
  }
  else cout << "Unable to open file\n";
  
  return;
}

string obj::getString(){
  string str = "NumPoints: ";
  //char* temp;
  //itoa(nPoints, temp, 10);
  //str.append(temp);
  str.append("\n");
  
  return str;
};

string obj::getTotalString(){
  string str = "Number of Objects: ";
  //char* temp;
  //itoa(nObjects, temp, 10);
  //str.append(temp);
  str.append("\n");
  
  return str;
}

obj::~obj(){
//  free(pointList);
}

void obj::save(char* filename){
  ofstream file(filename);
  if (file.is_open()){
    file << nObjects;
    for (int i = 0; i < nObjects; i++){
      file << "\n" << objectList[i].nPoints;
      for (int j = 0 ; j < objectList[i].nPoints; j++){
        file << "\n" << objectList[i].pointList[j].x << " " << objectList[i].pointList[j].y;
      }
    }
    
    file.close();
  }
  else cout << "failed to open save file";
}

/*
 * gets the calculated centroid. average of all points
 */
obj::pnt obj::getCentroid(){
  float x = 0;
  float y = 0;
  for (int i = 0; i < nPoints; i++){
    x += pointList[i].x;
    y += pointList[i].y;
  }
  
  x /= nPoints;
  y /= nPoints;
  
  pnt c;
  c.x = (int)(x+.5);
  c.y = (int)(y+.5);
  return c;
}

/*
 * move the object by x and y
 */
void obj::translation(int x, int y){
  for (int i = 0; i < nPoints; i++){
    pointList[i].x += x;
    pointList[i].y += y;
  }
}

/*
 * scales object from the centroid
 */
void obj::scale(float a, float b){
  pnt c = getCentroid();
  
  //const for centroid scaling. also +.5 for rounding
  const double px = c.x *(1-a) + .5, py = c.y * (1 - b) + .5;
  
  //iterate through each point updating their locations for scale
  for(int i = 0; i < nPoints; i++){
    pointList[i].x = (int)(pointList[i].x * a + px);
    pointList[i].y = (int)(pointList[i].y * b + py);
  }
}

/*
 * rotates object around the centroid
 */
void obj::rotation(float alpha){
  pnt c = getCentroid();
  
  //const for sin and cosin calculations
  const double radians = alpha*PI/180;
  const double sa = sin(radians), ca = cos(radians);
  //const for centroid rotation. also +.5 for rounding
  const double px = c.y*sa - c.x*ca + c.x+.5, py = c.y - c.x*sa - ca*c.y +.5;
  
  for(int i = 0; i < nPoints; i++){
    int x = (int)(pointList[i].x*ca - pointList[i].y*sa + px);
    pointList[i].y = (int)(pointList[i].x*sa + pointList[i].y*ca + py);
    pointList[i].x = x;
  }
}

typedef struct clippedPoint{
  obj::pnt p;
  int ABRL;
} cpnt;

int setABRL(cpnt* p, int xmin, int xmax, int ymin, int ymax){
       if (p->p.x < xmin) p->ABRL = 0b0001;
  else if (p->p.x > xmax) p->ABRL = 0b0010;
  else                    p->ABRL = 0;

       if (p->p.y < ymin) p->ABRL |= 0b0100;
  else if (p->p.y > ymax) p->ABRL |= 0b1000;

  return p->ABRL;
}

cpnt getEdgePoint(const cpnt in, const cpnt out, const int xmin, const int xmax, const int ymin, const int ymax, bool BAmode){
  cpnt p;
  p.ABRL = 0;
  
  int dx, dy = out.p.y - in.p.y;
  
  //if horizontal
  if(!dy){
    p.p.y = in.p.y;
    if(out.ABRL & 0b0010) p.p.x = xmax;
    else p.p.x = xmin;
  }
  //if vertical
  else if (!(dx = out.p.x - in.p.x)){
    p.p.x = in.p.x;
    if(out.ABRL & 0b1000) p.p.y = ymax;
    else p.p.y = ymin;
  }
  //if m=1
  else if (dy == dx){
    if(out.ABRL & 0b1000){
      p.p.y = ymax;
      dy = ymax - in.p.y;
    }
    else if (out.ABRL & 0b0100){
      p.p.y = ymin;
      dy = ymin - in.p.y;
    }
    
    if(out.ABRL & 0b0010){
      p.p.x = xmax;
      dx = xmax - in.p.x;
    }
    else if(out.ABRL & 0b0001){
      p.p.x = xmin;
      dx = xmin - in.p.x;
    }
    
    if (abs(dx) > abs(dy)) p.p.x = in.p.x + dy;
    else p.p.y = in.p.y + dx;
  }
  //if m=-1
  else if (dy == -dx){
    if(out.ABRL & 0b1000){
      p.p.y = ymax;
      dy = ymax - in.p.y;
    }
    else if (out.ABRL & 0b0100){
      p.p.y = ymin;
      dy = ymin - in.p.y;
    }
    
    if(out.ABRL & 0b0010){
      p.p.x = xmax;
      dx = xmax - in.p.x;
    }
    else if(out.ABRL & 0b0001){
      p.p.x = xmin;
      dx = xmin - in.p.x;
    }
    
    if (abs(dx) > abs(dy)) p.p.x = in.p.x - dy;
    else p.p.y = in.p.y - dx;
  }
  else{
    if(BAmode){

    }
    else{
      double m = ((double)dy)/dx;
      
      //if point is above
      if(out.ABRL & 0b1000){
        p.p.y = ymax;
        p.p.x = (ymax - in.p.y) / m + in.p.x;
        if (!setABRL(&p, xmin, xmax, ymin, ymax)) return p;
      }
      //if point is below
      //mutually exclusive from above
      else if(out.ABRL & 0b0100){
        p.p.y = ymin;
        p.p.x = (ymin - in.p.y) / m + in.p.x;
        if (!setABRL(&p, xmin, xmax, ymin, ymax)) return p;
      }

      //if the new point is still off to a side
      
      //if point is right
      if(p.ABRL & 0b0010){
        p.p.x = xmax;
        p.p.y = (xmax - in.p.x) / m + in.p.y;
      }
      //if point is left
      //mutually exclusive from right
      else if (p.ABRL & 0b0001){
        p.p.x = xmin;
        p.p.y = (xmin - in.p.x) / m + in.p.y;
      }
    }
  }
  
  return p;
}

typedef struct insideLine{
  bool isInside;
  cpnt p1, p2;
} iline;

inline bool isIn(obj::pnt p, const int xmin, const int xmax, const int ymin, const int ymax){
  return p.x >= xmin && p.x <= xmax && p.y >= ymin && p.y <= ymax;
}

iline getInsidePoints(const cpnt a, const cpnt b, const int xmin, const int xmax, const int ymin, const int ymax, bool BAmode){
  obj::line l = obj::line(a.p, b.p, BAmode);
  iline output;
  output.isInside = false;
  
  for (int i = 0; i < l.getNumPoints(); i++){
    obj::pnt p = l.getPoint(i);
    if(isIn(p, xmin, xmax, ymin, ymax)){
      if(!output.isInside){
        output.isInside = true;
        cpnt cp;
        cp.p = p;
        cp.ABRL = 0;
        output.p1 = cp;
      }
    }
    else{
      if(output.isInside){
        cpnt cp;
        cp.p = l.getPoint(i-1);
        cp.ABRL = 0;
        output.p2 = cp;
        return output;
      }
    }
  }
  
  return output;
}

typedef list<cpnt>::iterator ITR;

enum workingEdge{YMIN=0b0100, XMIN=0b0001, YMAX=0b1000, XMAX=0b0010};

//When crossing the clipping edge get the point along the edge
cpnt getEdgePoint(const cpnt a, const cpnt b, const workingEdge we, bool BAmode,
        const int xmin, const int xmax, const int ymin, const int ymax){
  int dx = b.p.x - a.p.x;
  int dy = b.p.y - a.p.y;
  cpnt p;
  
  
  if(we == YMIN || we == YMAX){
    int edgeVal = (we==YMIN?ymin:ymax);
    p.p.y = edgeVal;
    //special case vertical
    if(dx == 0) p.p.x = a.p.x;
    //special case m=1
    else if(dx == dy) p.p.x = a.p.x + dy;
    //special case m=-1
    else if(dx == -dy) p.p.x = a.p.x - dy;
    //BA
    else if(BAmode);
    //DDA
    else p.p.x = a.p.x + (int)((edgeVal - a.p.y) / (((double)dy)/dx) + .5);
  }
  else{
    int edgeVal = (we==XMIN?xmin:xmax);
    p.p.x = edgeVal;
    //special case vertical
    if(dy == 0) p.p.y = a.p.y;
    //special case m=1
    else if(dx == dy) p.p.y = a.p.y + dx;
    //special case m=-1
    else if(dx == -dy) p.p.y = a.p.y - dx;
    //BA
    else if(BAmode);
    //DDA
    else p.p.y = a.p.y + (int)((edgeVal - a.p.x) / (((double)dx)/dy) + .5);
  }
  
  return p;
}

void eliminateExtraPoints(list<cpnt>* lPnt, workingEdge we){
  //cout<<"test l1\n";
  for(ITR it = lPnt->begin(); it != lPnt->end(); ){
    //cout<<"test l2\n";
    if((*it).ABRL & we){/*cout<<"del "<<(*it).ABRL<<endl;*/it=lPnt->erase(it); }
    else{it++;/*cout<<"dont\n";*/}
    //cout<<"test l3\n";
  }
  //cout<<"test l4\n";
}

void clipAlongEdge(list<cpnt>* lPnt, int location, workingEdge we, 
        const int xmin, const int xmax, const int ymin, const int ymax, bool BAmode){
  //cout<<"teste "<<location<<"\n";
  if(location & we){
    //cout<<"needs clipping in "<<we<<endl;
    ITR it = lPnt->begin();
    cpnt b = *it;
    cpnt a;
    
    //cout<<"b"<<b.p.x<<' '<<b.p.y<<' '<<b.ABRL<<endl;
    //check between first and last points for edge cross
    //Only if object is not a line or point
    if(lPnt->size() > 2){
      a = lPnt->back();

      //cout<<a.ABRL<<' '<<b.ABRL<<' '<<we<<endl;
      if(we & ((a.ABRL & ~b.ABRL) | (~a.ABRL & b.ABRL))){
        //cout<<"fclip\n";      
        cpnt cp = getEdgePoint(a, b, we, BAmode, xmin,xmax,ymin,ymax);
        setABRL(&cp,xmin,xmax,ymin,ymax);
        lPnt->insert(it, cp);
        //cout<<"clip!\n";
      }
    }
    
    it++;
    
    for(; it != lPnt->end(); it++){
      a = b;
      b = *it;


      //cout<<a.ABRL<<' '<<b.ABRL<<' '<<we<<endl;
      //if we are crossing out of the 
      if(we & ((a.ABRL & ~b.ABRL) | (~a.ABRL & b.ABRL))){
        //cout<<"nclip\n";      
        cpnt cp = getEdgePoint(a, b, we, BAmode, xmin,xmax,ymin,ymax);
        setABRL(&cp,xmin,xmax,ymin,ymax);
        lPnt->insert(it, cp);
        //cout<<"clip!\n";
      }
    }
    
    //cout<<"test elim "<<lPnt->size()<<endl;
    eliminateExtraPoints(lPnt, we);
    //cout<<"test elim2 "<<lPnt->size()<<endl;
  }
  
}

/*
 * Clip object around rectangular bounds
 */
obj obj::clip(const int xmin, const int xmax, const int ymin, const int ymax){
  //cpnt* cp = new cpnt[nPoints];
  int location = 0;
  list<cpnt> lPnt;

  //cout << "testc1\n";
  //load points into list
  for(int i = 0; i < nPoints; i++){
    cpnt cp;
    cp.p = pointList[i];
    location |= setABRL(&cp, xmin, xmax, ymin, ymax);
    lPnt.push_back(cp);
  }
  
  
  //actually do the clipping
  
  //cout << "testc2\n";
  //cut off @ ymin
  clipAlongEdge(&lPnt, location, YMIN, xmin, xmax, ymin, ymax, false);
  //cout << "testc3\n";
  //cut off @ ymax
  clipAlongEdge(&lPnt, location, YMAX, xmin, xmax, ymin, ymax, false);
  //cout << "testc4\n";
  //cut off @ xmin
  clipAlongEdge(&lPnt, location, XMIN, xmin, xmax, ymin, ymax, false);
  //cout << "testc5\n";
  //cut off @ xmax
  clipAlongEdge(&lPnt, location, XMAX, xmin, xmax, ymin, ymax, false);
  
  //cout << "testc6\n";
  

  //object out of Viewport!
  if(lPnt.empty()) return obj(0,0);

  //convert list into a new object and return it
  //cout << "testc2 "<<lPnt.size()<<endl;
  ITR it = lPnt.begin();
  pnt* arr = new pnt[lPnt.size()];
  for(int i = 0 ; i < lPnt.size(); i++){
    arr[i] = (*it).p;
    //cout <<"testc3 "<<arr[i].x<<endl;
    it++;
  }

  return obj(lPnt.size(), arr);
}

