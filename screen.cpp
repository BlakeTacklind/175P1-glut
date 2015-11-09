/* 
 * File:   screen.cpp
 * Author: btacklind
 * 
 * Created on October 20, 2015, 12:36 PM
 */

#include "screen.h"
#include "object3D.h"
#include <list>
#include <iostream>
#include <math.h>
#include "userInterface.h"
#include "types.h"
#include "OpenGLhandler.h"
#include "object3Dsurface.h"

using namespace std;

list<screen*> screen::screenList;

screen::screen(int x, int y, int ofX, int ofY, pnt3 vec, float viewDist, void (*mkPix)(int, int, pnt3))
:offsetX(ofX), offsetY(ofY){
  width = x;
  height = y;
  viewDistance = viewDist;
  mp = mkPix;
  setNormal(vec);
  
  screenList.push_back(this);
}

void screen::setNormal(const pnt3& vec){
  if (vec == zeroVector){
    userInterface::printError("can't use zero vector as normal vector");
    return;
  }

  normal = ~vec;
  if(normal == unitX || normal == unitY || normal == unitZ) return;

  outx = ~(normal%unitZ);
  outy = normal%outx;

  if(outy.z < 0){
    outx = -outx;
    outy = -outy;
  }
}

screen::screen(const screen& orig) {}

screen::~screen() {}

void screen::freeAll(){
  for(list<screen*>::iterator it = screenList.end(); it != screenList.begin(); it--)
    delete *it;
  
  screenList.clear();
}

void screen::bufferAllScreens() {
  for(list<screen*>::iterator it = screenList.begin(); it != screenList.end(); it++){
    (*it)->bufferObjects();
  }
}

bool screen::compareSurfaces(const surface& first, const surface& second) {
  return ((second.getCentroid() - first.getCentroid()) * normal) > 0;  
}

void screen::bufferObjects() {
  //get all surfaces
  list<surface*> surfaces;
  for(int i = 0; i < object3Dsurface::getNumObjects(); i++){
    object3Dsurface* o = object3Dsurface::getObject(i);
    for(int j = 0; j < o->getNumSurface(); j++){
      surfaces.push_back(o->getSurface(j));
    }
  }
  
  //surface elimination
  {
    list<surface*>::iterator it = surfaces.begin();
    
    while(it != surfaces.end())
      if((normal)*(*it)->getNormal() <= 0) surfaces.erase(it);
      else  it++;
  }
  
  if(surfaces.empty()) return;
  
  //sort surfaces for display
  surfaces.sort(compareSurfaces);
  
  //scale and translate lines to fill screen
  
  //get max and mins of the 2d points
  float xmin, xmax, ymin, ymax;
  {
    {
      pntf temp = convert3dPoint(surfaces.front()->getParent()->getPoint(0));
      xmin = xmax = temp.x;
      ymin = ymax = temp.y;
    }
    
    for(list<surface*>::iterator it = surfaces.begin(); it != surfaces.end(); it++) {
      object3Dsurface* o = (*it)->getParent();
      list<unsigned int> l;
      for(int i = 0; i < (*it)->getNumPoints(); i++){
        l.push_front((*it)->getPntNum(i));
      }
      
      l.unique();
      
      for(list<unsigned int>::iterator it = l.begin(); it != l.end(); it++){
        pntf temp = convert3dPoint(o->getPoint(*it));
        
             if(temp.x > xmax) xmax = temp.x;
        else if(temp.x < xmin) xmin = temp.x;

             if(temp.y > ymax) ymax = temp.y;
        else if(temp.y < ymin) ymin = temp.y;
      }
    }
  }
  
  //find scale value
  float scale;
  {
    float scaleX = (width  - 1) / (xmax - xmin);
    float scaleY = (height - 1) / (ymax - ymin);
    scale = scaleX>scaleY?scaleY:scaleX;
  }
  
  for(list<surface*>::iterator it = surfaces.begin(); it != surfaces.end(); it++){
    for(int i = 0; i < (*it)->getNumPoints(); i++){
      pnt3 p3 = (*it)->getParent()->getPoint((*it)->getPntNum(i));
      pntf fp = convert3dPoint(p3);
      MakePix(fp.x * scale + xmin, fp.y * scale + ymin, 
              getColor(p3, (*it)->getParent()->getPointNormal((*it)->getPntNum(i))));
    }
  }
  
  /*
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
    pntf** p = new pntf*[2];
    
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
    float scaleX = (width  - 1) / (xmax - xmin);
    float scaleY = (height - 1) / (ymax - ymin);
    scale = scaleX>scaleY?scaleY:scaleX;
  }
  
  it = edge2d.begin();
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
  }*/
}

/*
 * get a color at a location using the Phong lighting model
 */
pnt3 screen::getColor(pnt3 location, pnt3 norm) {
  //light vector
  pnt3 l = ~(location - OpenGLhandler::getLpos());
  //reflection vector
  pnt3 r = l - 2 * norm << l;
          //Ka*Ia +
  return OpenGLhandler::getAmbiant() * OpenGLhandler::getIa() + 
          //Il / (||f - p|| + K) *
          OpenGLhandler::getIl() / (viewDistance + OpenGLhandler::getK()) * 
          //(kd * (L dot n) +
          (OpenGLhandler::getDiffuse() * (l * norm) + 
          //ks * (r dot v)^n)
          OpenGLhandler::getSpecular() * pow((r * normal), OpenGLhandler::getLightSize()));
}

/*
 * convert 3d point into a 2d coordinate system
 * using parallel projection
 */
pntf screen::convert3dPoint(pnt3 p){
  pntf r;
  if(normal == unitX){
    r.x = p.y;
    r.y = p.z;
  }
  else if(normal == unitY){
    r.x = -p.x;
    r.y = p.z;
  }
  else if(normal == unitZ){
    r.x = p.x;
    r.y = p.y;
  }
  else{
    r.x=outx*p;
    r.y=outy*p;

    //cout<<"test "<<r->x<<" "<<r->y<<endl;
  }

  return r;
}

void screen::drawLine(line* l){
  const int dist = l->getNumPoints();
  for(int i = 0; i < dist; i++){
    pnt p = l->getPoint(i);
    MakePix(p.x, p.y, {1,1,1});
  }

  //clean up line after usage
  delete l;
}
