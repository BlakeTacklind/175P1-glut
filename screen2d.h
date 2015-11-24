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

  pixelSelectionHelper getNearestPoint(int x, int y);
  pixelSelectionHelper getNearestLine(int x, int y);
  
  pntf translate(int x, int y);

  static screen2d* findScreen(int, int);

  static void freeAll();
  static void drawAll();

private:
  MakePixFunc* makePix;
  unsigned int width, height;

  float xMin, xMax, yMin, yMax;
  float scale;

  static list<screen2d*> screenList;
};


#endif	/* SCREEN2D_H */

