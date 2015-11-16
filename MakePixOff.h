

#ifndef MAKEPIXOFF_H
#define MAKEPIXOFF_H

#include "types.h"
#include "OpenGLhandler.h"
#include "MakePixFunc.h"

class MakePixOff: public MakePixFunc{
public:
  MakePixOff(int offsetX, int offsetY):offx(offsetX), offy(offsetY){};

  void operator()(int x, int y, pnt3 color){OpenGLhandler::MakeCPix(x+offx, y+offy, color);};

private:
  const int offx, offy;
};

#endif

