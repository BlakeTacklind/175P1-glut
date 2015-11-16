

#ifndef MAKEMPIXOFF_H
#define MAKEMPIXOFF_H

#include "types.h"
#include "OpenGLhandler.h"
#include <math.h>
#include "MakePixFunc.h"

class MakeMPixOff: public MakePixFunc{
public:
  MakeMPixOff(int offsetX, int offsetY, pnt3 color):offx(offsetX), offy(offsetY), tone(color){};

  void operator()(int x, int y, pnt3 color){OpenGLhandler::MakeMPix(x+offx, y+offy, getIntensityFromColor(color), tone);};

private:
  inline unsigned int getIntensityFromColor(pnt3 c){return (unsigned int)(length(c)/(sqrt(3)/10));};

  const int offx, offy;
  const pnt3 tone;
};

#endif

