/* 
 * File:   screen.h
 * Author: btacklind
 *
 * Created on October 20, 2015, 12:36 PM
 */

#ifndef SCREEN_H
#define	SCREEN_H

#include "types.h"

class screen {
public:
  screen(int x, int y, pnt3 vec, void (*mkPix)(int, int));
  screen(const screen& orig);
  virtual ~screen();
  
  void displayObjects();
private:
  int width;
  int height;
  pnt3 vector;
  
  void (*MakePix)(int, int);
};

#endif	/* SCREEN_H */

