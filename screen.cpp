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
#include "cline.h"

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

bool screen::compareSurfaces::operator()(surface*& first, surface*& second) {
  return ((second->getCentroid() - first->getCentroid()) * viewVec) > 0;  
}


void screen::fillLine(cpnt& a, cpnt& b){
  line l(dg(a), dg(b), true);

  pnt3 m = (b.c-a.c)/l.getNumPoints();

  for(int i = 1; i < l.getNumPoints() - 1; i++){
    pnt x = l.getPoint(i);
    cpnt c = {x.x, x.y, a.c+(i*m)};

    MakePix(x.x, x.y, c.c);
  }


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
  
  {
    list<surface*>::iterator it = surfaces.begin();
    while(it != surfaces.end()){
      //cout<<"<"<<normal.x<<", "<<normal.y<<", "<<normal.z<<"> * <"<<((*it)->getNormal()).x<<", "<<((*it)->getNormal()).y<<", "<<((*it)->getNormal()).z<<"> = "<< (normal * ((*it)->getNormal()))<<endl;
      if((normal * ((*it)->getNormal())) <= 0){
        it = surfaces.erase(it);
        //cout<<"deleted"<<endl;
      }
      else 
        it++;
    }
  }
  

  //cout<<"test 4\n";
  if(surfaces.empty()) return;
  
  //cout<<"test 4.5\n";
  //sort surfaces for display
  surfaces.sort(compareSurfaces(normal));

  {
    //cout<<"Normal list\n";
    //for(list<surface*>::iterator it = surfaces.begin(); it != surfaces.end(); it++)
      //cout<<"<"<<((*it)->getNormal()).x<<", "<<((*it)->getNormal()).y<<", "<<((*it)->getNormal()).z<<">"<<endl;
  }
  //cout<<"test 5\n";
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
  //cout<<"test 6\n";
  
  //find scale value
  float scale;
  {
    float scaleX = (width  - 1) / (xmax - xmin);
    float scaleY = (height - 1) / (ymax - ymin);
    scale = scaleX>scaleY?scaleY:scaleX;
  }
  
  for(list<surface*>::iterator it = surfaces.begin(); it != surfaces.end(); it++){

    //cout<<"surface has "<<(*it)->getNumPoints()<<" points\n";
    cpnt* cpnts;
    if(OpenGLhandler::getDrawMode() != OpenGLhandler::points) cpnts = new cpnt[(*it)->getNumPoints()];

    for(int i = 0; i < (*it)->getNumPoints(); i++){
      pnt3 p3 = (*it)->getParent()->getPoint((*it)->getPntNum(i));
      pntf fp = convert3dPoint(p3);

      if(OpenGLhandler::getDrawMode() != OpenGLhandler::points){
        cpnts[i].x = scale * (fp.x - xmin);
        cpnts[i].y = scale * (fp.y - ymin);
        cpnts[i].c = getColor(p3, (*it)->getParent()->getPointNormal((*it)->getPntNum(i)));

        //MakePix(cpnts[i].x, cpnts[i].y, cpnts[i].c);
      }
      else{
        MakePix(scale * (fp.x - xmin), scale * (fp.y - ymin),
          getColor(p3, (*it)->getParent()->getPointNormal((*it)->getPntNum(i))));
      }
    }

    if(OpenGLhandler::getDrawMode() == OpenGLhandler::points) continue;

    if (OpenGLhandler::getLightModel() == OpenGLhandler::Phong){
      cout << "Not Implemented Phong" <<endl;
      continue;
    }

    list<cline*> CLlist;

    CLlist.push_back(new cline(cpnts[0], cpnts[(*it)->getNumPoints()-1]));

    for(int i = 1; i < (*it)->getNumPoints(); i++)
      CLlist.push_back(new cline(cpnts[i], cpnts[i-1]));
    
    //cout<<"Printing "<<CLlist.size()<<" lines\n";
    //for(list<cline*>::iterator it2 = CLlist.begin(); it2 != CLlist.end(); it2++){
      //cout<<(*it2)->getP1().x<<" "<<(*it2)->getP1().y<<" "<<(*it2)->getP2().x<<" "<<(*it2)->getP2().y<<endl;
    //}

    if(OpenGLhandler::getDrawMode() == OpenGLhandler::lines)
      for(list<cline*>::iterator it2 = CLlist.begin(); it2 != CLlist.end(); it2++)
        (*it2)->draw(MakePixOff(offsetX, offsetY));
    else if(OpenGLhandler::getDrawMode() == OpenGLhandler::fill)
      cline::raster(MakePixOff(offsetX, offsetY), CLlist);

    for(list<cline*>::iterator it2 = CLlist.begin(); it2 != CLlist.end(); it2++)
      delete *it2;
    
    delete [] cpnts;
  }
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
