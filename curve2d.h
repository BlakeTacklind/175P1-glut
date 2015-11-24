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
#include <fstream>

using namespace std;

class curve2d {
public:
  enum curveType {Bezier, BSpline};

  static bool load(char* filename);
  static bool save(char* filename);
  inline static bool load(){load(storedFileName);};
  inline static bool save(){save(storedFileName);};
  
  curve2d(unsigned int nPnts, pntf* cPnts);
  virtual ~curve2d();
  
  inline pntf getControlPoint(unsigned int i){return controlPoints[i];};
  inline unsigned int getNumControlPoints(){return nPoints;};
  
  virtual void addPoint(unsigned int pos, pntf loc);
  virtual void removePoint(unsigned int);
  void modifyPoint(unsigned int p, pntf loc);

  pntf* getMinMax();
  
  virtual pntf* draw(unsigned int resolution)=0;
  virtual pnt3 getColor()=0;

  inline static char* getStoredFile(){return storedFileName;};
  static void freeAll();
  static unsigned int getNumCurves(){return nCurves;};
  static curve2d* getCurve(unsigned int i){return curveList[i];};

  inline unsigned int getNumPoints(){return nPoints;};
  
protected:
  inline pntf* getControlPoints(){return controlPoints;};
  virtual bool canRemove()=0;

  curveType myType;
  
private:
  static bool closeStuff(ifstream&, list<curve2d*>&, bool);

  static char* storedFileName;
  static curve2d** curveList;
  static unsigned int nCurves;

  unsigned int nPoints;
  pntf* controlPoints;
};


class bezier : public curve2d{
public:
  inline bezier(unsigned int nPnts, pntf* cPnts):curve2d(nPnts, cPnts){myType = Bezier;};
  
  inline bool canRemove(){return getNumPoints()>2;};
  pnt3 getColor(){return {1,0,0};};
  
  pntf* draw(unsigned int resolution);
};

class bSpline : public curve2d{
public:
  bSpline(unsigned int nPnts, pntf* cPnts, unsigned int order);
  bSpline(unsigned int nPnts, pntf* cPnts, unsigned int order, float* r);
  
  override void addPoint(unsigned int pos, pntf loc);
  override void removePoint(unsigned int);
  
  inline bool canRemove(){return k<getNumPoints();};
  
  pntf* draw(unsigned int resolution);
  pnt3 getColor(){return {0,1,0};};

  inline unsigned int getK(){return k;};
  inline float getU(unsigned int i){return u[i];};

private:
  //inline float getU(unsigned int i){return u[i];};
  unsigned int getSegment(float f);
  static float* getDefaultKnots(unsigned int);
  
  unsigned int k;
  float* u;

};


#endif	/* CURVE2D_H */

