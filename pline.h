/* 
 * File:   pline.h
 * Author: btacklind
 *
 * Holder for Phong lines
 */

#ifndef PLINE_H
#define	PLINE_H

#include "types.h"
#include "line.h"
#include <list>
#include "MakePixOff.h"
#include "screen.h"

using namespace std;

class pline: public line{
public:
  pline(pnt P1, pnt P2, pnt3 A, pnt3 B, pnt3 norm, screen* printTo);
  pline();
  pline(const pline& orig);
  virtual ~pline();
  
  inline pnt3 getA(){return a;};
  inline pnt3 getB(){return b;};
  
  inline pnt3 getPnt3(unsigned int i){pnt3 m=(b-a)/(numPoints-1);return a+m*i;};

  void draw(MakePixOff mk);
  static void raster(MakePixOff mk, list<pline*>& lst);

private:
  void Fill();
  
  holder* getHolderFromI(int* I);
  
  unsigned int numPoints;
  
  pnt3 a, b;
  pnt3 normal;
  
  screen* scrn;
  
  pnt3* fill;
  
  struct pntPair{
    pnt3 p;
    int x;
  };
  
  struct holder{
    pntPair max, min;
  };

};

#endif	/* PLINE_H */

