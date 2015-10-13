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
 * Default constructor
 */
obj::obj(unsigned int numPoints, pnt* points){
  nPoints = numPoints;
  pointList = points;
}

obj::~obj(){
  //delete [] pointList;
}

/*
 * load object file
 */
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
      objectList = new obj[nObjects];
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
      pnt* points = new pnt[num];
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

/*
 * save objects to file
 */
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
 * fills in pixels of object using the passed function and appropriate 
 * algorithm mode
 */
void obj::fill(void (*MakePix)(int, int), bool BAmode){
  
  //if object is a polygon get the list of lines
  if(getNumPoints() > 2){
    //initialize rastering edges
    yMin = yMax = getPoints()[0].y;
    xMin = xMax = getPoints()[0].x;

    list<line*> lLine;
    
    //get line from first and last point
    lLine.push_front(new line(getPoints()[0], getPoints()[getNumPoints()-1], BAmode));
    
    //get all other lines
    for(int j = 1; j < getNumPoints(); j++){
      //add lines to list
      lLine.push_front(new line(getPoints()[j], getPoints()[j-1], BAmode));
      
      //update outside edge box
           if(getPoints()[j].y > yMax) yMax = getPoints()[j].y;
      else if(getPoints()[j].y < yMin) yMin = getPoints()[j].y;

           if(getPoints()[j].x > xMax) xMax = getPoints()[j].x;
      else if(getPoints()[j].x < xMin) xMin = getPoints()[j].x;
      
    }
    
    //scan through all horizontal lines in object
    for (int i = yMin; i <= yMax; i++){
      list<line*> temp(lLine);
      //initialize draw parity
      bool draw = false;
      
      //shorten the list to lines that are in this horizontal scan
      shortenList(temp, i);
      
      bool *Draw = new bool[2];

      //scan each pixel in scan line
      for(int j = xMin; j <= xMax; j++){
        //Get current draw state for findInList
        Draw[0] = draw;
        
        //find lines at point, remove them from list, and indicate a change
        //in draw parity
        findInList(temp, j, i, Draw);
        
        //if found odd number of lines flip the parity
        if(Draw[0]) draw = !draw;
        
        //if found a line or are on draw parity make a pixel
        if(draw || Draw[1]) MakePix(j, i);
        
      }
      
      //clean up
      delete [] Draw;
    }

    //free line space created
    for(list<line*>::iterator it = lLine.begin(); it != lLine.end(); it++){
      delete *it;
    }
    
  }
  //if object was a line draw line object
  else if(getNumPoints() == 2){
    line* l = new line(getPoints()[0], getPoints()[1], BAmode);
    for(int i = 0; i < l->getNumPoints(); i++){
      MakePix(l->getPoint(i).x, l->getPoint(i).y);
    }
    delete l;
  }
  //draw object if it is a pixel object
  else if(getNumPoints() == 1){
    MakePix(getPoints()[0].x, getPoints()[0].y);
  }

}

/*
 * find if a lines at (x, y) and indicate if we should switch parity
 * on input out[0] is if algorithm is currently drawing
 */
void obj::findInList(list<line*> &l, int x, int y, bool* out){
  const bool drawing = out[0];
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
    //find where line meets scan line, effectively
    //this works only because list has been shortened
    while((*it)->getPoint(i).y != y || 
            //continue searching if the line travels in x and we are currently drawing
            ((*it)->getXtravel() && drawing && 
            //and its the last point
            (i == (*it)->getNumPoints()-1 || 
            //or its the last point on this scan line
            (i < (*it)->getNumPoints()-1 && (*it)->getPoint(i+1).x != x))))
      i++;
    
    //check if line effectively crosses
    if((*it)->getPoint(i).x == x){
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
void obj::shortenList(list<line*> &l, int y){
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
 * clip all objects currently stored and put them into their own storage
 */
void obj::clipObjects(int xmin, int xmax, int ymin, int ymax){
  clippedObjects = new obj[nObjects];
  nClippedObjects = 0;

  for(int i = 0; i < nObjects; i++){
    obj o = objectList[i].clip(xmin, xmax, ymin, ymax);
    if(o.getNumPoints()) clippedObjects[nClippedObjects++] = o;
  }
}

void obj::freeAll(){
  for(int i=0; i < obj::nObjects; i++){
    delete [] obj::getObject(i).getPoints();
  }

  delete [] obj::objectList;
}

/* 
 * 2D Transformations
 *    translation, scale, and rotation about centroid
 */

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

/*
 * Clipping Section
 *    Using Sutherland-Hodgman Algorithm
 * 
 * The Plan:
 * For each line that crosses an edge add the point along the edge that is on
 * that line. Then remove all the points outside of viewport
 */

//A type for clipped points with Above, Below, Right, Left stored
typedef struct clippedPoint{
  obj::pnt p;
  int ABRL;
} cpnt;
//enum for the ABRL edge being worked
enum workingEdge{YMIN=0b0100, XMIN=0b0001, YMAX=0b1000, XMAX=0b0010};
//Iterator shortener
typedef list<cpnt>::iterator ITR;

//Set the ABRL of point give values
int setABRL(cpnt* p, int xmin, int xmax, int ymin, int ymax){
       if (p->p.x < xmin) p->ABRL = 0b0001;
  else if (p->p.x > xmax) p->ABRL = 0b0010;
  else                    p->ABRL = 0;

       if (p->p.y < ymin) p->ABRL |= 0b0100;
  else if (p->p.y > ymax) p->ABRL |= 0b1000;

  return p->ABRL;
}

//When crossing the clipping edge get the point along the edge
//TODO: BA algorithm
cpnt getEdgePoint(const cpnt a, const cpnt b, const workingEdge we, const bool BAmode,
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

/*
 * Remove points outside of the edge from the list
 */
void eliminateExtraPoints(list<cpnt>* lPnt, const workingEdge we){
  for(ITR it = lPnt->begin(); it != lPnt->end(); ){
    if((*it).ABRL & we) it=lPnt->erase(it); 
    else it++;
  }
}

/*
 * Clip 
 */
void clipAlongEdge(list<cpnt>* lPnt, const int location, const workingEdge we, 
        const int xmin, const int xmax, const int ymin, const int ymax, const bool BAmode){
  
  //if object is partially or completely out of the working edge
  if(location & we){
    
    ITR it = lPnt->begin();
    cpnt b = *it;
    cpnt a;
    
    //check between first and last points for edge cross
    //Only if object is not a line or point
    if(lPnt->size() > 2){
      a = lPnt->back();

      //if it crosses the edge
      if(we & ((a.ABRL & ~b.ABRL) | (~a.ABRL & b.ABRL))){
        //find the point between the two, on the edge
        cpnt cp = getEdgePoint(a, b, we, BAmode, xmin,xmax,ymin,ymax);
        setABRL(&cp,xmin,xmax,ymin,ymax);
        //add it to the list
        lPnt->insert(it, cp);
      }
    }
    
    it++;
    
    //for each pair of points
    for(; it != lPnt->end(); it++){
      a = b;
      b = *it;

      //if the line is crossing the edge
      if(we & ((a.ABRL & ~b.ABRL) | (~a.ABRL & b.ABRL))){
        //find the point between the two, on the edge
        cpnt cp = getEdgePoint(a, b, we, BAmode, xmin,xmax,ymin,ymax);
        setABRL(&cp,xmin,xmax,ymin,ymax);
        //add it to the list
        lPnt->insert(it, cp);
      }
    }
    
    //remove points outside of the edge
    eliminateExtraPoints(lPnt, we);
  }
  
}

/*
 * Clip this object around rectangular bounds
 */
obj obj::clip(const int xmin, const int xmax, const int ymin, const int ymax){
  int location = 0;
  list<cpnt> lPnt;

  //load points into list
  for(int i = 0; i < nPoints; i++){
    cpnt cp;
    cp.p = pointList[i];
    location |= setABRL(&cp, xmin, xmax, ymin, ymax);
    lPnt.push_back(cp);
  }
  
  //actually do the clipping
  
  //cut off @ ymin
  clipAlongEdge(&lPnt, location, YMIN, xmin, xmax, ymin, ymax, false);
  //cut off @ ymax
  clipAlongEdge(&lPnt, location, YMAX, xmin, xmax, ymin, ymax, false);
  //cut off @ xmin
  clipAlongEdge(&lPnt, location, XMIN, xmin, xmax, ymin, ymax, false);
  //cut off @ xmax
  clipAlongEdge(&lPnt, location, XMAX, xmin, xmax, ymin, ymax, false);

  //object out of Viewport!
  if(lPnt.empty()) return obj(0,0);

  //convert list into a new object and return it
  ITR it = lPnt.begin();
  pnt* arr = new pnt[lPnt.size()];
  for(int i = 0 ; i < lPnt.size(); i++){
    arr[i] = (*it).p;
    it++;
  }

  return obj(lPnt.size(), arr);
}

