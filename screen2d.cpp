/* 
 * File:   screen2d.cpp
 * Author: btacklind
 * 
 * Created on November 21, 2015, 2:16 PM
 */

#include "screen2d.h"
#include <list>
#include "curve2d.h"
#include "userInterface.h"
#include "line.h"
#include "OpenGLhandler.h"

using namespace std;

list<screen2d*> screen2d::screenList;

screen2d::screen2d(int x, int y, MakePixFunc* mkPix) {
  if(x <= 0 || y <= 0){
    userInterface::printError("screen size cannot be negative");
    return;
  }
  width = x;
  height = y;
  makePix = mkPix;

  screenList.push_back(this);
}

screen2d::screen2d(const screen2d& orig) {
}

screen2d::~screen2d() {
}

void screen2d::freeAll(){
  while(screenList.end() != screenList.begin()){
    delete screenList.front();
    screenList.pop_front();
  }
}

void screen2d::draw(){
  if (curve2d::getNumCurves() == 0) return;
  
  xMax = xMin = curve2d::getCurve(0)->getMinMax()[0].x;
  yMax = yMin = curve2d::getCurve(0)->getMinMax()[0].y;

  for (int i = 0; i < curve2d::getNumCurves(); ++i){
    curve2d* c = curve2d::getCurve(i);
    pntf* p = c->getMinMax();
    if(xMin > p[0].x) xMin = p[0].x;
    if(yMin > p[0].y) yMin = p[0].y;

    if(xMax < p[1].x) xMax = p[1].x;
    if(yMax < p[1].y) yMax = p[1].y;
  }
  
  float padding;
  {
    float paddingX = (xMax - xMin) / 10;
    float paddingY = (yMax - yMin) / 10;
    padding = paddingY>paddingX?paddingX:paddingY;
  }

  xMax += padding;
  yMax += padding;

  xMin -= padding;
  yMin -= padding;

  {
    float scaleX = (width  - 1) / (xMax - xMin);
    float scaleY = (height - 1) / (yMax - yMin);
    scale = scaleX>scaleY?scaleY:scaleX;
  }

  pntf minPoint = {xMin, yMin};

  for (int i = 0; i < curve2d::getNumCurves(); ++i){
    // cout<<"test 3\n";
    curve2d* c = curve2d::getCurve(i);
    pntf* pnts = c->draw(OpenGLhandler::getResolution());
    // cout<<"test 4\n"; 

    pntf p = (c->getControlPoint(0) - minPoint) * scale;
    // cout<<"p "<<p.x<<" "<<p.y<<" control "<<c->getControlPoint(0).x<<" "<<c->getControlPoint(0).y<<" minPoint "<<minPoint.x<<" "<<minPoint.y<<endl;
    pnt p1 = {(int)p.x, (int)p.y};

    for(int j = 0; j < c->getNumControlPoints(); j++){
      p = (c->getControlPoint(j) - minPoint) * scale;
      // cout<<"p "<<p.x<<" "<<p.y<<" control "<<c->getControlPoint(j).x<<" "<<c->getControlPoint(j).y<<" minPoint "<<minPoint.x<<" "<<minPoint.y<<endl;

      pnt p2 = {(int)p.x, (int)p.y};

      line l(p1, p2, true);

      
      for(int k = 0; k < l.getNumPoints(); k++){
        (*makePix)(l.getPoint(k).x, l.getPoint(k).y, {1,1,1});
      }

      p1 = p2;
    }

    p = (pnts[0] - minPoint) * scale;
    p1 = {(int)p.x, (int)p.y};

    // cout<<"test 5\n"; 
    for(int j = 1; j < OpenGLhandler::getResolution(); j++){
      
      p = (pnts[j] - minPoint) * scale;
      pnt p2 = {(int)p.x, (int)p.y};

      // cout<<"test 7 "<<p1.x<<" "<<p1.y<<endl; 
      // cout<<"test 7 "<<p2.x<<" "<<p2.y<<endl; 
      line l(p1, p2, true);

      // cout<<"test 8\n"; 
      
      for(int k = 0; k < l.getNumPoints(); k++){
        // cout<<l.getPoint(k).x<<" "<< l.getPoint(k).y<<" "<<minPoint.x<<" "<<minPoint.y<<endl;
        (*makePix)(l.getPoint(k).x, l.getPoint(k).y, c->getColor());
      }

      // cout<<"test 9\n"; 
      p1 = p2;
    }
  }
  // cout<<"test e\n";
}

void screen2d::drawAll(){
  for(list<screen2d*>::iterator it = screenList.begin(); it != screenList.end(); it++)
    (*it)->draw();
}

screen2d* screen2d::findScreen(int x, int y){
  return screenList.front();
}

pixelSelectionHelper screen2d::getNearestPoint(int x, int y){
  pixelSelectionHelper help = {0, 0, nullptr};
  if(!curve2d::getNumCurves()) return help;

  pntf p = translate(x,y);

  help.c = curve2d::getCurve(0);
  float minDist = dist(p, curve2d::getCurve(0)->getControlPoint(0));

  for(int i = 0; i < curve2d::getNumCurves(); i++){
    curve2d* c = curve2d::getCurve(i);
    for (int j = 0; j < c->getNumPoints(); ++j){
      float d = dist(p, c->getControlPoint(j));
      
      if(d < minDist){
        help.c = c;
        minDist = d;
        help.index = j;
      }
    }
  }

  help.distance = minDist*scale;
  p = help.c->getControlPoint(help.index);
  help.position = {(p.x - xMin) * scale, (p.y - yMin) * scale};

  return help;
}

pntf screen2d::translate(int x, int y) {
  return {(x/scale)+xMin, (y/scale)+yMin};
}

pixelSelectionHelper screen2d::getNearestLine(int x, int y) {
  pixelSelectionHelper help = {0, 0, nullptr};
  if(!curve2d::getNumCurves()) return help;
  
  pntf p = translate(x,y);

  help.c = curve2d::getCurve(0);
  float minDist = dist(p, curve2d::getCurve(0)->getControlPoint(0));

  for(int i = 0; i < curve2d::getNumCurves(); i++){
    curve2d* c = curve2d::getCurve(i);
    
    float d = dist(p, c->getControlPoint(0));
    if(d < minDist){
      help.c = c;
      minDist = d;
      help.index = 0;
    }
    
    d = dist(p, c->getControlPoint(c->getNumControlPoints()-1));
    if(d < minDist){
      help.c = c;
      minDist = d;
      help.index = c->getNumControlPoints();
    }
    
    for (int j = 1; j < c->getNumPoints(); ++j){
      d = dist(p, c->getControlPoint(j), c->getControlPoint(j-1));
      
      if(d < minDist){
        help.c = c;
        minDist = d;
        help.index = j;
      }
    }
  }

  help.distance = minDist*scale;
  p = help.c->getControlPoint(help.index);
  help.position = {(p.x - xMin) * scale, (p.y - yMin) * scale};

  return help;
}
