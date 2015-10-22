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
  inline static bool load(){load(storedFileName);};
  inline static bool save(){save(storedFileName);};
  
  object3D(int npnt, pnt3* Points, int nedge, edge* Edges);
  object3D();
  object3D(const object3D& orig);
  virtual ~object3D();
  
  static void freeAll();
  
  inline static unsigned int getNumObjects(){return nObjects;};
  inline static object3D* getObject(unsigned int i){return objectList[i];};
  inline unsigned int getNumPoints(){return nPoints;};
  inline unsigned int getNumEdges(){return nEdges;};
  pnt3** getEdge(unsigned int i);
  
  void translate(float x, float y, float z);
  void scale(float x, float y, float z);
  void rotate(pnt3 a, pnt3 b, float r);
  
private:
  static char* storedFileName;
  
  pnt3 getCentroid();
  
  static unsigned int nObjects;
  static object3D** objectList;
  
  unsigned int nPoints;
  pnt3* points;
  unsigned int nEdges;
  edge* edges;
};

#endif	/* OBJECT3D_H */

