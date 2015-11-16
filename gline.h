
#ifndef CLINE_H
#define CLINE_H

#include "types.h"
#include "line.h"
#include <list>
#include "MakePixFunc.h"

using namespace std;

class gline: public line{
  
public:
  gline(cpnt a, cpnt b);
  virtual ~gline();

  inline unsigned int getGNumPoints(){return numPoints;};
  inline cpnt getCPoint(unsigned int i){pnt x = getPoint(i); return {x.x, x.y, fill[i]};};
  
  inline cpnt getCP1(){return p1;};
  inline cpnt getCP2(){return p2;};

  void draw(MakePixFunc* mk);
  static void raster(MakePixFunc* mk, list<gline*>& lst);

private:

  void Fill();
  
  cpnt p1;
  cpnt p2;

  unsigned int numPoints;
  
  pnt3* fill;
};

#endif
