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
#include <list>

using namespace std;

class screen {
public:
  screen(int x, int y, int ofX, int ofY, pnt3 vec, void (*mkPix)(int, int));
  inline screen(){};
  screen(const screen& orig);
  virtual ~screen();

  static void freeAll();
  
  static void bufferAllScreens();
  void bufferObjects();
  
  inline static screen* getLastScreen(){return screenList.back();};
  inline int getScreenWidth (){return width; };
  inline int getScreenHeight(){return height;};

  inline pnt3 getNormal(){return normal;};
  void setNormal(const pnt3& p);
  
private:
  static list<screen*> screenList;
  
  pntf* convert3dPoint(pnt3* p);
  
  void drawLine(line* l);
  
  unsigned int width;
  unsigned int height;
  pnt3 normal, outy, outx;
  const int offsetX=0, offsetY=0;
  
  inline void MakePix(int x, int y){mp(x+offsetX, y+offsetY);}; 
  void (*mp)(int, int);
};

#endif	/* SCREEN_H */

