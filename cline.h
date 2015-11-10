
#ifndef CLINE_H
#define CLINE_H

#include "types.h"
#include "line.h"
#include <list>
#include "MakePixOff.h"

using namespace std;

class cline{
  
public:
  cline(cpnt a, cpnt b);
  cline(pnt a, pnt b, pnt3 norm);
  virtual ~cline();

  inline unsigned int getNumPoints(){return numPoints;};
  inline cpnt getPoint(unsigned int i){return {l->getPoint(i).x, l->getPoint(i).y, fill[i]};};
  
  inline cpnt getP1(){return p1;};
  inline cpnt getP2(){return p2;};

  void draw(MakePixOff mk);
  static void raster(MakePixOff mk, list<cline*>& lst);



private:

  int* findFrontBackAtLine(int y);

  void FillG();
  void FillP();
  
  cpnt p1;
  cpnt p2;

  line* l;

  unsigned int numPoints;
  
  pnt3* fill;
};

#endif
