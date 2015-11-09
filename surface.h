/* 
 * File:   surface.h
 * Author: btacklind
 *
 * Created on November 5, 2015, 2:29 PM
 */

#ifndef SURFACE_H
#define	SURFACE_H

#include "types.h"
#include "object3Dsurface.h"

class surface {
  friend class object3Dsurface;
public:
  surface();
  surface(unsigned int nPoints, int* pointNumbers, pnt3 normal);
  surface(const surface& orig);
  virtual ~surface();
  
  void updatePntNormals(pnt3* norms);
  
  void fill(void (*MakeCPix)(int, int, pnt3), pnt3 view);
  
  const char* getPntString();
  const char* getNormalString();
  
  inline pnt3 getNormal(){return norm;};
  
  static bool compare(const surface& first, const surface& second);
  
  inline void setParent(object3Dsurface* p){parent = p;};
  
private:
  pnt3* pnts;
  int* pntNums;
  unsigned int nPnts;
  object3Dsurface* parent;
  
  pnt3* pntNormals;
  
  pnt3 norm;
};

#endif	/* SURFACE_H */

