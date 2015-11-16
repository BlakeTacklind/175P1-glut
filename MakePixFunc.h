

#ifndef MAKEPIX_H
#define MAKEPIX_H

#include "types.h"

class MakePixFunc{
public:
  MakePixFunc(){};

  virtual void operator()(int x, int y, pnt3 color){};
};

#endif



