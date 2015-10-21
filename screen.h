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
  /*
  inline int getYmin(){return yMin;};
  inline int getYmax(){return yMax;};
  inline int getXmin(){return xMin;};
  inline int getXmax(){return xMax;};
  
  inline int setYmin(int i){yMin = i;};
  inline int setYmax(int i){yMax = i;};
  inline int setXmin(int i){xMin = i;};
  inline int setXmax(int i){xMax = i;};
  */
  inline int getScreenWidth (){return width; };
  inline int getScreenHeight(){return height;};

  void setNormal(const pnt3& p);
  
private:
  void bufferObjects();
  static list<screen*> screenList;
  
  pntf* convert3dPoint(pnt3* p);
  
  void drawLine(line* l);
  
  unsigned int width;
  unsigned int height;
  pnt3 normal, outy, outx;
  const int offsetX=0, offsetY=0;
  
  //viewport values
  //int xMin, yMin, xMax, yMax;
 
  inline void MakePix(int x, int y){mp(x+offsetX, y+offsetY);}; 
  void (*mp)(int, int);
};

#endif	/* SCREEN_H */

