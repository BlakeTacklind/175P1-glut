/* 
 * File:   screen.cpp
 * Author: btacklind
 * 
 * Created on October 20, 2015, 12:36 PM
 */

#include "screen.h"
#include "object3D.h"

screen::screen(int x, int y, pnt3 vec, void (*mkPix)(int, int)) {
  width = x;
  height = y;
  vector = vec;
  MakePix = mkPix;
}

screen::screen(const screen& orig) {
}

screen::~screen() {
}

void screen::bufferObjects() {
  //get 3d object edges
  list<pnt3**> edge3d;
  for (int i = 0; i < object3D::getNumObjects(); i++){
    object3D* o = object3D::getObject(i);
    for(int j = 0; j < o->getNumEdges(); j++){
      edge3d.push_back(o->getEdge(j));
    }
  }
  
  //If there are NO edges, don't display anything
  if(!edge3d.size()) return;
  
  //convert to 2d lines float lines
  list<pntf**> edge2d;
  for (list<pnt3**>::iterator it = edge3d.begin(); it != edge3d.end(); it++){
    pntf** p = new (pntf*)[2];
    
    p[0] = convert3dPoint((*it)[0]);
    p[1] = convert3dPoint((*it)[1]);
    
    edge2d.push_back(p);
    
    delete [] (*it);
  }
  
  //find the min and max
  float xmin, xmax, ymin, ymax;
  list<pntf**>::iterator it = edge2d.begin();
  xmin = xmax = (*it)[0]->x;
  ymin = ymax = (*it)[0]->y;
  while(it != edge2d.end()){
         if((*it)[0]->x > xmax) xmax = (*it)[0]->x;
    else if((*it)[0]->x < xmin) xmin = (*it)[0]->x;
    
         if((*it)[1]->x > xmax) xmax = (*it)[1]->x;
    else if((*it)[1]->x < xmin) xmin = (*it)[1]->x;
    
         if((*it)[0]->y > ymax) ymax = (*it)[0]->y;
    else if((*it)[0]->y < ymin) ymin = (*it)[0]->y;
    
         if((*it)[1]->y > ymax) ymax = (*it)[1]->y;
    else if((*it)[1]->y < ymin) ymin = (*it)[1]->y;
    
    it++;
  }
  
  //scale and translate lines to fill screen
  
  //find scale value
  float scale;
  {
    float scaleX = width  / (xmax - xmin);
    float scaleY = height / (ymax - ymin);
    scale = scaleX>scaleY?scaleY:scaleX;
  }
  
  list<pntf**>::iterator it = edge2d.begin();
  while(it != edge2d.end()){
    pnt a;
    pnt b;
    
    //translate then scale points to integer counterparts
    a.x = (int)(scale*((*it)[0]->x - xmin));
    a.y = (int)(scale*((*it)[0]->y - ymin));
    b.x = (int)(scale*((*it)[1]->x - xmin));
    b.y = (int)(scale*((*it)[1]->y - ymin));
    
    //clean up array
    delete (*it)[0];
    delete (*it)[1];
    delete [] (*it);
    
    //draw the line created by the two integer points
    drawLine(new line(a, b, true));
    
    it++;
  }
}

/*
 * convert 3d point into a 2d coordinate system
 * using parallel projection
 */
pntf* screen::convert3dPoint(pnt3* p){
  pntf* r = new pntf;
  if(vector == unitX){
    r.x = p->y;
    r.y = p->z;
  }
  else if(vector == unitY){
    r.x = -p->x;
    r.y =  p->z;
  }
  else if(vector == unitZ){
    r.x = p->x;
    r.y = p->y;
  }
  else{
    
  }
  
  return r;
}

void screen::drawLine(line* l){
  const int dist = l->getNumPoints();
  for(int i = 0; i < dist; i++){
    pnt p = l->getPoint(i);
    MakePix(p.x, p.y);
  }
  
  //clean up line after usage
  delete l;
}

