

#ifndef MAKEMPIXOFF_H
#define MAKEMPIXOFF_H

#include "types.h"
#include "OpenGLhandler.h"
#include <math.h>
#include "MakePixFunc.h"

class MakeMPixOff: public MakePixFunc{
public:
  MakeMPixOff(int offsetX, int offsetY):offx(offsetX), offy(offsetY){};

  void operator()(int x, int y, pnt3 color){OpenGLhandler::MakeMPix(x+offx, y+offy, getIntensityFromColor(color));};

private:
  inline unsigned int getIntensityFromColor(pnt3 c){return (unsigned int)(length(c)/(sqrt(3)/10));};

  const int offx, offy;
};

#endif

