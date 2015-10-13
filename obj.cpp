#include "obj.h"
#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>
#include <math.h>
#include <list>
#include <cstring>
#include "line.h"

#define PI 3.14159265

using namespace std;

unsigned int obj::nObjects;
obj* obj::objectList;
char* obj::storedFileName;
unsigned int obj::nClippedObjects;
obj* obj::clippedObjects;

/*
 * Does a complex check
 */
bool AllCheck(line* l, int i, int x, int y, bool drawing){
  //if we found a match and we are not drawing or looking at a line that travels vertically
  if((!drawing || !l->getXtravel()) && l->getPoint(i).x == x) return true;
  //however if the line travels in x direction and we are currently drawing
  //we want to switch at the last point on that line
  return (drawing && l->getXtravel() && l->getPoint(i).x == x && (i == l->getNumPoints()-1 || (i < l->getNumPoints()-1 && l->getPoint(i+1).x != x)));
}

/*
 * find if a lines at (x, y) and indicate if we should switch parity
 * on input out[0] is if algorithm is currently drawing
 */
void findInList(list<line*> &l, int x, int y, bool* out){
  bool output1 = false;
  bool output2 = false;
  list<line*>::iterator it = l.begin();
  list<line*> passedList;
  
  
  for(; it != l.end();){
    //if the line being check is horizontal.
    //A simple check can be performed to see it the point is inside
    //This assumes the list has been shortened
    if((*it)->isHorizontal() && x >= (*it)->getP1().x && x <= (*it)->getP2().x){
      //set to draw on and mark to draw at least this point
      out[0] = !out[0];
      out[1] = true;
      
      //TODO: move element to front of list for faster drawing later
      //TODO: remove from list if scan has moved beyond line

      return;
    }
    
    int i = 0;
    //find where line meets scan line
    //TODO: change so effective x,y is found with x traveling lines
    //this works only because list has been shortened
    while((*it)->getPoint(i).y != y) i++;
    
    //check if line effectively crosses
    if(AllCheck((*it), i, x, y, out[0])){
      //add line to list of lines passed at this point
      passedList.push_front(*it);      

      //don't let line switch the parity again on this scan line
      it = l.erase(it);

      //change parity
      output1 = !output1;
      //mark that scan passed a line
      output2 = true;
    }
    else it++;

  }
  
  /*
   * In the case of a complex point:
   *   indicated by more then one line passing through point
   * 
   * Theory:
   * At any single complex point we examine what happens above and below
   * the the point. Each pair of lines that are on one side of above and
   * below the point indicates "in for one pixel" condition. If this isn't
   * the case switch drawing parity.
   * A line through the point is BOTH above and below. Two lines meeting
   * on a vertex at the point is when it may or may not switch parity. 
   * If the lines are on the same side of the horizontal -> in for one
   * or the lines are on the opposite sides -> switch parity
   * 
   * The precondition that there are no horizontal lines is important
   * In practice:
   * Since the number of lines above + below is even. we only need to check
   * even and odd relation of one side. In this case top. Due to straight lines
   * there may be only one or the other point top. The other will be bottom or
   * on the line. 
   */
  if(passedList.size() > 1){
    int above = 0;
    for(it = passedList.begin(); it != passedList.end(); it++)
      if((*it)->getP1().y > y || (*it)->getP2().y > y) above++;    
    
    output1 = above % 2;
  } 

  out[0] = output1;
  out[1] = output2;
}

/*
 * Shortens the list of lines to be scanned across by checking their heights
 * vs the current scan line
 */
void shortenList(list<line*> &l, int y){
  list<line*>::iterator it = l.begin();
  
  while(it != l.end()){
    int y1 = (*it)->getP1().y;
    int y2 = (*it)->getP2().y;
    
    //ensure y1 is the smaller y value
    if(y1 > y2){
      int temp = y2;
      y2 = y1;
      y1 = temp;
    }

    //check too keep is line on scan line
    if(y > y2 || y < y1) it = l.erase(it);
    else it++;
  }
}

/*
 * fills in pixels of object using the passed function and appropriate 
 * algorithm mode
 */
void obj::fill(void (*MakePix)(int, int), bool BAmode){


      //cout<<"testb1\n";
      //for all polygons get the non-horizontal lines
  if(getNumPoints() > 2){
    yMin = yMax = getPoints()[0].y;
    xMin = xMax = getPoints()[0].x;

    list<line*> lLine;

        //cout<<"testb2\n";
    //get line from first and last point
    
    //  if (getPoints()[0].y - getPoints()[getNumPoints()-1].y)
    lLine.push_front(new line(getPoints()[0], getPoints()[getNumPoints()-1], BAmode));
    

    //cout<<"testb5 " <<o.getNumPoints()<<"\n";
    //get lines from polygon
    for(int j = 1; j < getNumPoints(); j++){
      //cout<<"testb6\n";
          
          //if not horizontal add line to list
      //if (getPoints()[j].y - getPoints()[j-1].y){
      lLine.push_front(new line(getPoints()[j], getPoints()[j-1], BAmode));
           if(getPoints()[j].y > yMax) yMax = getPoints()[j].y;
      else if(getPoints()[j].y < yMin) yMin = getPoints()[j].y;

           if(getPoints()[j].x > xMax) xMax = getPoints()[j].x;
      else if(getPoints()[j].x < xMin) xMin = getPoints()[j].x;
            //printList(lLine);
      //}
         // cout<<"testb7\n";
    }
      //scan through horizontal lines
    for (int i = yMin; i <= yMax; i++){
      list<line*> temp(lLine);
      //cout<<"test "<<i<<endl;
      bool draw = false;
      
      //printList(temp);

      //shorten the list to lines that are in this horizontal scan
      shortenList(temp, i);
      //printList(temp);
      
      //cout<<"tests "<<temp.size()<<endl;
      //scan each pixel for a line
      bool *Draw = new bool[2];

      for(int j = xMin; j <= xMax; j++){
        //cout<<"tests1 "<< j << ' '<< i<<endl;
        findInList(temp, j, i, Draw);
        //cout<<"tests2\n";
        
        //if found odd number of lines flip the parity
        if(Draw[0]){
          //cout<<"-----------------test "<<j<<' '<<i<<endl;
          draw = !draw;//?false:true;
        }
        
        //if found a line or are on draw parity make a pixel
        if(draw || Draw[1]) MakePix(j, i);
        //cout<<"tests3\n";
        Draw[0] = draw;
      }
      //cout <<"tests4\n";
      delete [] Draw;
    }

    //free line space created
    for(list<line*>::iterator it = lLine.begin(); it != lLine.end(); it++){
      delete *it;
    }
        //cout<<"testb4\n";
  }
  //draw line object
  else if(getNumPoints() == 2){
    line* l = new line(getPoints()[0], getPoints()[1], BAmode);
    for(int i = 0; i < l->getNumPoints(); i++){
      MakePix(l->getPoint(i).x, l->getPoint(i).y);
    }
    //drawLine(*l);
    delete l;
  }
      //draw pixel object
  else if(getNumPoints() == 1){
    MakePix(getPoints()[0].x, getPoints()[0].y);
  }

      //cout<<"testb8 "<<obj::getNumClippedObjects()<<endl;

    
    //cout << "test third\n";

}

/*
obj::obj(const obj& orig){
  nPoints = orig.nPoints;
  pointList = new pnt[nPoints];//(pnt*)malloc(nPoints * sizeof(pnt));
  memcpy(pointList, orig.pointList, nPoints);
}
*/
/*
obj::obj& operator= (const obj& other){
  if(this != &other){
    nPoints = other.nPoints;
    pointList = new pnt[nPoints];
    memcpy(pointList, other.pointList, nPoints);
  }

  return *this;
}
*/
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

void obj::freeAll(){
  for(int i=0; i < obj::nObjects; i++){
    delete [] obj::getObject(i).getPoints();
  }

  delete [] obj::objectList;
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
      objectList = new obj[nObjects];//(obj*)malloc(sizeof(obj) * nObjects);
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
      pnt* points = new pnt[num];//(pnt*)malloc(sizeof(pnt) * num);
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
  //free(pointList);
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
  line l = line(a.p, b.p, BAmode);
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
    else if(dx == dy) p.p.x = a.p.x + edgeVal - a.p.y;
    //special case m=-1
    else if(dx == -dy) p.p.x = a.p.x - edgeVal + a.p.y;
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
    else if(dx == dy) p.p.y = a.p.y + edgeVal - a.p.x;
    //special case m=-1
    else if(dx == -dy) p.p.y = a.p.y - edgeVal + a.p.y;
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

