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

using namespace std;

class screen2d {
public:
  screen2d(int x, int y, MakePixFunc* mkPix);
  screen2d(const screen2d& orig);
  virtual ~screen2d();

  void draw();

  static void freeAll();
  static void drawAll();

private:
  MakePixFunc* makePix;
  unsigned int width, height;


  static list<screen2d*> screenList;
};

#endif	/* SCREEN2D_H */

