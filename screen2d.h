/* 
 * File:   screen2d.h
 * Author: btacklind
 *
 * Created on November 21, 2015, 2:16 PM
 */

#ifndef SCREEN2D_H
#define	SCREEN2D_H

#include "types.h"
#include "MakePixFunc.h"
#include <list>
#include "curve2d.h"
#include "line.h"

using namespace std;

typedef struct {
  unsigned int distance, index;
  curve2d* c;
  pnt position;
} pixelSelectionHelper;

class screen2d {
public:
  screen2d(int x, int y, MakePixFunc* mkPix);
  screen2d(const screen2d& orig);
  virtual ~screen2d();

  void draw();
  void draw(curve2d*);
  line* draw(line*);

  pixelSelectionHelper getNearestPoint(int x, int y);
  pixelSelectionHelper getNearestLine(int x, int y);
  
  pntf translate(int x, int y);
  float translateX(int y);

  inline static screen2d* getMainScreen(){return mainScreen;};
  inline static screen2d* getHelpScreen(){return helpScreen;};

  static void freeAll();
  static void drawAll();
  
  inline static void setMainScreen(screen2d* s){mainScreen = s;};
  inline static void setHelpScreen(screen2d* s){helpScreen = s;};

private:
  MakePixFunc* makePix;
  unsigned int width, height;

  float xMin, xMax, yMin, yMax;
  float scale;

//  static list<screen2d*> screenList;
  static screen2d* mainScreen;
  static screen2d* helpScreen;
};


#endif	/* SCREEN2D_H */

