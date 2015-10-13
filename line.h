
#ifndef LINE_H
#define LINE_H

#include "obj.h"

class line{
  
public:
  line(obj::pnt a, obj::pnt b, bool BAmode);
  virtual ~line();
  line(const line &orig);
  //line& operator= (const line& other);

  inline unsigned int getNumPoints(){return xTravel?dx:dy;};
  inline obj::pnt getPoint(unsigned int i){return fill[i];};
  inline bool isHorizontal(){return !dy;};
  
  inline obj::pnt getP1(){return p1;};
  inline obj::pnt getP2(){return p2;};
  
private:
  void Fill(bool BAmode);
  
  obj::pnt p1;
  obj::pnt p2;
  
  int dx;
  int dy;
  
  bool xTravel;
  
  obj::pnt* fill;
};

#endif
