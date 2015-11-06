/* 
 * File:   surface.h
 * Author: btacklind
 *
 * Created on November 5, 2015, 2:29 PM
 */

#ifndef SURFACE_H
#define	SURFACE_H

#include "types.h"

class surface {
public:
  surface();
  surface(unsigned int nPoints, pnt3* points, pnt3 normal);
  surface(const surface& orig);
  virtual ~surface();
  
  void updatePntNormals(pnt3* norms);
  
  void fill(void (*MakeCPix)(int, int, pnt3));
  
  char* getPntString();
  char* getNormalString();
  
private:
  pnt3* pnts;
  unsigned int nPnts;
  
  pnt3* pntNormals;
  
  pnt3 norm;
};

#endif	/* SURFACE_H */

