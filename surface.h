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

class object3Dsurface;

class surface {
public:
  surface();
  surface(unsigned int nPoints, unsigned int* pointNumbers, pnt3 normal);
  surface(const surface& orig);
  virtual ~surface();
  
  void updatePntNormals(pnt3* norms);
  
  void fill(void (*MakeCPix)(int, int, pnt3), pnt3 view);
  
  const char* getPntString();
  const char* getNormalString();
  
  inline pnt3 getNormal(){return norm;};
  
  pnt3 getCentroid();
  
  inline void setParent(object3Dsurface* p){parent = p;};
  inline object3Dsurface* getParent()   {return parent;};
  
  inline unsigned int getNumPoints(){return nPnts;};
  inline unsigned int getPntNum(int i){return pntNums[i];};
  
private:
  pnt3* pnts;
  unsigned int* pntNums;
  unsigned int nPnts;
  object3Dsurface* parent;
  
  pnt3* pntNormals;
  
  pnt3 norm;
};

#endif	/* SURFACE_H */

