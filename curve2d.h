/* 
 * File:   curve2d.h
 * Author: btacklind
 *
 * Created on November 21, 2015, 2:20 PM
 */

#ifndef CURVE2D_H
#define	CURVE2D_H

#include "types.h"
#include "MakePixFunc.h"
#include <list>

class curve2d {
public:
  
  static bool load(char*);
  static bool save(char*);
  inline static bool load(){load(savedFileName);};
  inline static bool save(){save(savedFileName);};
  
  curve2d(unsigned int nPnts, pntf* cPnts);
  virtual ~curve2d();
  
  inline pntf getControlPoint(unsigned int i){return controlPoints[i];};
  
  void addPoint(unsigned int pos, pntf loc);
  void removePoint(unsigned int);
  void modifyPoint(unsigned int p, pntf loc);
  
  virtual pntf* draw(unsigned int resolution) = 0;
  
protected:
  inline pntf* getControlPoints(){return controlPoints;};
  inline unsigned int getNumPoints(){return nPoints;};
  virtual bool canRemove()=0;
  
private:
  static char* savedFileName;
  static list<curve2d*> curveList;
  
  unsigned int nPoints;
  pntf* controlPoints;
};

class bezier : public curve2d{
public:
  inline bezier(unsigned int nPnts, pntf* cPnts):curve2d(nPnts, cPnts){};
  
  inline bool canRemove(){return true;};
  
  pntf* draw(unsigned int resolution);
};

class bSpline : public curve2d{
public:
  bSpline(unsigned int nPnts, pntf* cPnts, unsigned int order);
  bSpline(unsigned int nPnts, pntf* cPnts, unsigned int order, float* r);
  
  inline bool canRemove(){return k<getNumPoints();};
  
  pntf* draw(unsigned int resolution);

private:
  //inline float getU(unsigned int i){return u[i];};
  unsigned int getSegment(float f);
  
  unsigned int k;
  float* u;

};


#endif	/* CURVE2D_H */

