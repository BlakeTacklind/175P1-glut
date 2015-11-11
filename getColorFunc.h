

#ifndef GETCOLORFUNC_H
#define GETCOLORFUNC_H

#include "types.h"
#include "OpenGLhandler.h"
#include <math.h>

class getColorFunc{
public:
  getColorFunc(pnt3 viewVec, float viewDist): view(viewVec), viewDistance(viewDist){};

  pnt3 operator()(pnt3 location, pnt3 norm){  
  //light vector
  pnt3 l = ~(location - OpenGLhandler::getLpos());
  //reflection vector
  pnt3 r = l - 2 * norm << l;
          //Ka*Ia +
  return OpenGLhandler::getAmbiant() * OpenGLhandler::getIa() + 
          //Il / (||f - p|| + K) *
          OpenGLhandler::getIl() / (viewDistance + OpenGLhandler::getK()) * 
          //(kd * (L dot n) +
          (OpenGLhandler::getDiffuse() * (l * norm) + 
          //ks * (r dot v)^n)
          OpenGLhandler::getSpecular() * pow((r * view), OpenGLhandler::getLightSize()));
  };

private:
  const pnt3 view;
  const float viewDistance;
};

#endif

