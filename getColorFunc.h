

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
  pnt3 l = ~(OpenGLhandler::getLpos() - location);
  //reflection vector
  pnt3 r = (2 * (norm << l)) - l;
          //Ka*Ia +
  pnt3 c= OpenGLhandler::getAmbiant() * OpenGLhandler::getIa() + 
          //Il / (||f - p|| + K) *
          OpenGLhandler::getIl() / (viewDistance + OpenGLhandler::getK()) * 
          //(kd * (L dot n) +
          (OpenGLhandler::getDiffuse() * (l * norm) + 
          //ks * (r dot v)^n)
          OpenGLhandler::getSpecular() * pow((r * view), OpenGLhandler::getLightSize()));
  
  if(c.x > 1){c / c.x; cout<<"x big\n";}
  if(c.y > 1){c / c.y; cout<<"y big\n";}
  if(c.z > 1){c / c.z; cout<<"z big\n";}

  if(c.x < 0 || c.y < 0 || c.z < 0)
    cout<<"loc ("<<location.x<<", "<<location.y<<", "<<location.z<<") light <"<<l.x<<", "<<l.y<<", "<<l.z<<"> normal<"<<norm.x<<", "<<norm.y<<", "<<norm.z<<"> view <"<<view.x<<", "<<view.y<<", "<<view.z<<"> color {"<<c.x<<", "<<c.y<<", "<<c.z<<"}\n";

  return {1,1,1};
  };

private:
  const pnt3 view;
  const float viewDistance;
};

#endif

