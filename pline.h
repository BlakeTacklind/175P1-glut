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
#include "MakePixFunc.h"
#include "screen.h"
#include "getColorFunc.h"

using namespace std;

class pline: public line{
 
  struct pntPair{
    pnt3 p;
    int x;
  };
  
  typedef struct h{
    struct pntPair max, min;
  } holder;

public:
  pline(pnt P1, pnt P2, pnt3 A, pnt3 B, pnt3 norm, getColorFunc);
  pline();
  pline(const pline& orig);
  virtual ~pline();
  
  inline pnt3 getA(){return a;};
  inline pnt3 getB(){return b;};
  
  inline pnt3 getPnt3(unsigned int i){pnt3 m=(b-a)/(numPoints-1);return a+m*i;};

  void draw(MakePixFunc* mk);
  static void raster(MakePixFunc* mk, list<pline*>& lst, getColorFunc);

private:
  void Fill(getColorFunc);
  
  holder* getHolderFromI(int* I);
  
  unsigned int numPoints;
  
  pnt3 a, b;
  pnt3 normal;
  
  pnt3* fillp;
 };

#endif	/* PLINE_H */

