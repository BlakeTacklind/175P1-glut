/* 
 * File:   screen.h
 * Author: btacklind
 *
 * Created on October 20, 2015, 12:36 PM
 */

#ifndef SCREEN_H
#define	SCREEN_H

#include "types.h"
#include "line.h"
#include "surface.h"
#include <list>
#include "MakePixFunc.h"

using namespace std;

class screen {
public:
  screen(int x, int y, pnt3 vec, float viewDist, MakePixFunc* mkPix);
  inline screen(){};
  screen(const screen& orig);
  //virtual ~screen();

  static void freeAll();
  
  static void bufferAllScreens();
  void bufferObjects();
  
  inline static screen* getLastScreen(){return screenList.back();};
  inline int  getScreenWidth (){return width; };
  inline int  getScreenHeight(){return height;};


  inline pnt3 getNormal(){return normal;};
  void setNormal(const pnt3& p);
  
  pnt3 getColor(pnt3 location, pnt3 norm);

private:
  static list<screen*> screenList;

  void fillLine(cpnt& a, cpnt& b);
  
  pntf convert3dPoint(pnt3 p);

  void surfaceElimination(list<surface*>& surfaces);

  
  float viewDistance;
  
  unsigned int width;
  unsigned int height;
  pnt3 normal, outy, outx;
  const int offsetX=0, offsetY=0;
  
  MakePixFunc* MakePix;
  
  struct pntHolder{
    pnt3 real;
    pnt rel;
    pnt3 vec;
  };

  //bool compareSurfaces(const surface& first, const surface& second);
  class compareSurfaces{
  public:
    compareSurfaces(pnt3 view):viewVec(view){};

    bool operator()(surface*& first, surface*& second);

  private:
    const pnt3 viewVec;
  };
};

#endif	/* SCREEN_H */

