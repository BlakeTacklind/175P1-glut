
#ifndef LINE_H
#define LINE_H

#include "types.h"
#include <iostream>

using namespace std;

class line{
  
public:
  line(pnt a, pnt b, bool BAmode);
  inline line(){cout<<"should not happen\n";};
  virtual ~line();

  inline unsigned int getNumPoints(){return (xTravel?dx:dy) + 1;};
  inline pnt getPoint(unsigned int i){return fill[i];};
  inline bool isHorizontal(){return !dy;};
  inline bool getXtravel(){return xTravel;};
  
  inline pnt getP1(){return p1;};
  inline pnt getP2(){return p2;};
  
protected:
  int* findFrontBackAtLine(int y);
  
private:
  void Fill(bool BAmode);
  
  pnt p1;
  pnt p2;
  
  int dx;
  int dy;
  
  bool xTravel;
  
  pnt* fill;
};

#endif
