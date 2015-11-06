/* 
 * File:   object3Dsurface.h
 * Author: btacklind
 *
 * Created on November 5, 2015, 3:20 PM
 */

#ifndef OBJECT3DSURFACE_H
#define	OBJECT3DSURFACE_H

#include "surface.h"

class object3Dsurface {
public:
  static bool load(const char* filename);
  static bool save(const char* filename);
  inline static bool load(){load(storedFileName);};
  inline static bool save(){save(storedFileName);};

  inline static char* getStoredFile(){return storedFileName;};
  
  object3Dsurface(int npnt, pnt3* Points, int nedge, edge* Edges, int nsurf, surface** surfs);
  object3Dsurface();
  object3Dsurface(const object3Dsurface& orig);
  virtual ~object3Dsurface();
private:
  void findPointNormals;
  
  static char* storedFileName;
  
  static unsigned int nObjects;
  static object3Dsurface** objectList;
  
  unsigned int nPoints;
  pnt3* points;
  unsigned int nEdges;
  edge* edges;
  unsigned int nSurface;
  surface** surfaces;
  pnt3* pntNorms;
};

#endif	/* OBJECT3DSURFACE_H */

