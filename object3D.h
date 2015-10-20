/* 
 * File:   object3D.h
 * Author: btacklind
 *
 * Created on October 20, 2015, 12:05 PM
 */

#ifndef OBJECT3D_H
#define	OBJECT3D_H

#include "types.h"
#include "object2D.h"
#include <list>

class object3D {
public:
  static bool load(const char* filename);
  static bool save(const char* filename);
  inline static bool load(){load(lastFileUsed);};
  inline static bool save(){save(lastFileUsed);};
  
  object3D();
  object3D(const object3D& orig);
  virtual ~object3D();
  
  //unsure return and handed types
  void convertTo2D(pnt3 vector);
  
  void translate(float x, float y, float z);
  void rotate(float x, float y, float z);
  void scale(float x, float y, float z);
  
private:
  static char* lastFileUsed;
  
  pnt3 getCentroid();
  
  static list<object3D*> objectList;
  
  unsigned int nPoints;
  pnt3* points;
  unsigned int nEdge;
  edge* edge;
};

#endif	/* OBJECT3D_H */

