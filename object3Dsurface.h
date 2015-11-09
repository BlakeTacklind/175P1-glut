/* 
 * File:   object3Dsurface.h
 * Author: btacklind
 *
 * Created on November 5, 2015, 3:20 PM
 */

#ifndef OBJECT3DSURFACE_H
#define	OBJECT3DSURFACE_H

#include "surface.h"
#include <list>
#include <fstream>
#include <iostream>

using namespace std;

class object3Dsurface {
public:
  static bool load(const char* filename);
  static bool save(const char* filename);
  inline static bool load(){load(storedFileName);};
  inline static bool save(){save(storedFileName);};

  inline static char* getStoredFile(){return storedFileName;};
  
  object3Dsurface(int npnt, pnt3* Points, pnt3* pNormals, int nedge, edge* Edges, int nsurf, surface** surfs);
  object3Dsurface();
  object3Dsurface(const object3Dsurface& orig);
  virtual ~object3Dsurface();
  
  inline static unsigned int getNumObjects(){return nObjects;};
  inline static object3Dsurface* getObject(int i){return objectList[i];};
  
  inline unsigned int getNumSurface(){return nSurface;};
  inline surface* getSurface(int i){return surfaces[i];};
  
  inline pnt3 getPoint(int i){return points[i];};
  
  inline pnt3 getPointNormal(int i){return pntNorms[i];};
  
private:
  static bool close(ifstream& f, list<object3Dsurface*>& tList, bool ret);

  
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

