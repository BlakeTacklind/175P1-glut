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

class screen2d {
public:
  screen2d(int x, int y, MakePixFunc* mkPix);
  screen2d(const screen2d& orig);
  virtual ~screen2d();
private:
  MakePixFunc* makePix;
};

#endif	/* SCREEN2D_H */

