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
#include "gline.h"
#include "pline.h"
#include "getColorFunc.h"

using namespace std;

list<screen*> screen::screenList;

screen::screen(int x, int y, pnt3 vec, float viewDist, MakePixFunc* mkPix){
  width = x;
  height = y;
  viewDistance = viewDist;
  MakePix = mkPix;
  setNormal(vec);
  
  screenList.push_back(this);
}

void screen::setNormal(const pnt3& vec){
  if (vec == zeroVector){
    userInterface::printError("can't use zero vector as normal vector");
    return;
  }

  normal = ~vec;
  if(normal == unitZ || normal == -unitZ) return;

  outx = ~(normal%unitZ);
  outy = normal%outx;

  if(outy.z < 0){
    outx = -outx;
    outy = -outy;
  }
}

screen::screen(const screen& orig) {}

//screen::~screen() {}

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

bool screen::compareSurfaces::operator()(surface*& first, surface*& second) {
  return ((second->getCentroid() - first->getCentroid()) * viewVec) > 0;  
}


void screen::fillLine(cpnt& a, cpnt& b){
  line l(dg(a), dg(b), true);

  pnt3 m = (b.c-a.c)/l.getNumPoints();

  for(int i = 1; i < l.getNumPoints() - 1; i++){
    pnt x = l.getPoint(i);
    cpnt c = {x.x, x.y, a.c+(i*m)};

    (*MakePix)(x.x, x.y, c.c);
  }


}

void screen::surfaceElimination(list<surface*>& surfaces){
  list<surface*>::iterator it = surfaces.begin();

  while(it != surfaces.end())
    if((normal * ((*it)->getNormal())) >= 0)
      it = surfaces.erase(it);
    else it++;
  
}

void screen::bufferObjects() {
  //cout<<"test 2\n";
  //get all surfaces
  list<surface*> surfaces;
  for(int i = 0; i < object3Dsurface::getNumObjects(); i++){
    object3Dsurface* o = object3Dsurface::getObject(i);
    for(int j = 0; j < o->getNumSurface(); j++){
      surfaces.push_back(o->getSurface(j));
    }
  }

  //surface elimination
  surfaceElimination(surfaces);
  

  if(surfaces.empty()) return;
  
  //sort surfaces for display
  surfaces.sort(compareSurfaces(normal));

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

    cpnt* cpnts;
    //pntHolder* spnts;
    if(OpenGLhandler::getDrawMode() != OpenGLhandler::points && 
            OpenGLhandler::getLightModel() == OpenGLhandler::Gouraud)
      cpnts = new cpnt[(*it)->getNumPoints()];
    /* NOT NECESSARY FOR PROJECT 3
    else if(OpenGLhandler::getDrawMode() != OpenGLhandler::points && 
            OpenGLhandler::getLightModel() == OpenGLhandler::Phong)
      spnts = new pntHolder[(*it)->getNumPoints()];
    */

    //get value of surface vertices
    for(int i = 0; i < (*it)->getNumPoints(); i++){
      pnt3 p3 = (*it)->getParent()->getPoint((*it)->getPntNum(i));
      pntf fp = convert3dPoint(p3);

      if(OpenGLhandler::getDrawMode() == OpenGLhandler::points){
        (*MakePix)(scale * (fp.x - xmin), scale * (fp.y - ymin),
          getColor(p3, (*it)->getParent()->getPointNormal((*it)->getPntNum(i))));
      }
      else if (OpenGLhandler::getLightModel() == OpenGLhandler::Gouraud){
        cpnts[i].x = scale * (fp.x - xmin);
        cpnts[i].y = scale * (fp.y - ymin);
        cpnts[i].c = getColor(p3, (*it)->getParent()->getPointNormal((*it)->getPntNum(i)));
      }
      /* NOT NECESSARY FOR PROJECT 3
      else if( OpenGLhandler::getLightModel() == OpenGLhandler::Phong){
        int num = (*it)->getPntNum(i);
        spnts[i] = {(*it)->getParent()->getPoint(num), 
            {(int)(scale * (fp.x - xmin)), (int)(scale * (fp.y - ymin))}, 
            (*it)->getParent()->getPointNormal(num)};
        MakePix(spnts[i].rel.x, spnts[i].rel.y, getColor(p3, (*it)->getParent()->getPointNormal((*it)->getPntNum(i))));
      }
      */
    }

    //skip rest if only displaying points
    if(OpenGLhandler::getDrawMode() == OpenGLhandler::points) continue;

    //perform Phong Algorithm
    /* NOT NECESSARY FOR PROJECT 3
    if (OpenGLhandler::getLightModel() == OpenGLhandler::Phong){
      list<pline*> PLlist;
      
      PLlist.push_back(new pline(spnts[0].rel, spnts[(*it)->getNumPoints()-1].rel,
              spnts[0].real, spnts[(*it)->getNumPoints()-1].real,
              ~(spnts[0].vec + spnts[(*it)->getNumPoints()-1].vec) , 
              getColorFunc(normal, viewDistance)));
      
      PLlist.back()->draw(MakePixOff(offsetX, offsetY));
      pline* ln;
      for(int i = 1; i < (*it)->getNumPoints(); i++){
        
        PLlist.push_back(new pline(spnts[i].rel, spnts[i-1].rel,
                spnts[i].real, spnts[i-1].real,
                ~(spnts[i].vec + spnts[i-1].vec) ,
                getColorFunc(normal, viewDistance)));
                
        
        PLlist.back()->draw(MakePixOff(offsetX, offsetY));
      }
      
      //Raster if nessesary
      if(OpenGLhandler::getDrawMode() == OpenGLhandler::fill)
        pline::raster(MakePixOff(offsetX, offsetY), PLlist, getColorFunc(normal, viewDistance));
      
      for(list<pline*>::iterator it2 = PLlist.begin(); it2 != PLlist.end(); it2++)
        delete (*it2);
    }
    */
    //perform Gouraud algorithm
    if(OpenGLhandler::getLightModel() == OpenGLhandler::Gouraud){
      list<gline*> CLlist;

      CLlist.push_back(new gline(cpnts[0], cpnts[(*it)->getNumPoints()-1]));

      for(int i = 1; i < (*it)->getNumPoints(); i++)
        CLlist.push_back(new gline(cpnts[i], cpnts[i-1]));


      if(OpenGLhandler::getDrawMode() == OpenGLhandler::lines)
        for(list<gline*>::iterator it2 = CLlist.begin(); it2 != CLlist.end(); it2++)
          (*it2)->draw(MakePix);
      else if(OpenGLhandler::getDrawMode() == OpenGLhandler::fill)
        gline::raster(MakePix, CLlist);

      for(list<gline*>::iterator it2 = CLlist.begin(); it2 != CLlist.end(); it2++)
        delete *it2;

      delete [] cpnts;
    }
  }
}

/*
 * get a color at a location using the Phong lighting model
 */
pnt3 screen::getColor(pnt3 location, pnt3 norm) {
  return getColorFunc(normal, viewDistance)(location, norm);
}

/*
 * convert 3d point into a 2d coordinate system
 * using parallel projection
 */
pntf screen::convert3dPoint(pnt3 p){
  pntf r;
  if(normal == unitZ){
    r.x = p.x;
    r.y = p.y;
  }
  else if(normal == -unitZ){
    r.x = p.x;
    r.y = p.y;
  }
  else{
    r.x=outx*p;
    r.y=outy*p;
  }

  return r;
}
