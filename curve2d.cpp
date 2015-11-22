/* 
 * File:   curve2d.cpp
 * Author: btacklind
 * 
 * Created on November 21, 2015, 2:20 PM
 */

#include "curve2d.h"
#include "userInterface.h"
#include <math.h>

curve2d::curve2d(unsigned int nPnts, pntf* cPnts) {
  nPoints = nPnts;
  controlPoints = cPnts;
}

curve2d::~curve2d() {
  delete [] controlPoints;
}

void curve2d::addPoint(unsigned int pos, pntf loc) {
  if(pos > nPoints){
    userInterface::printError("insert position not possible");
    return;
  }
  
  pntf* c = new pntf[++nPoints];
  
  int i = 0;
  for(; i < pos; i++)
    c[i] = controlPoints[i];
  
  c[i++] = loc;
  
  for(; i < nPoints; i++)
    c[i] = controlPoints[i-1];
  
  delete [] controlPoints;
  controlPoints = c;
}

void curve2d::modifyPoint(unsigned int p, pntf loc) {
  if(p >= nPoints){
    userInterface::printError("point does not exist");
    return;
  }
  
  controlPoints[p] = loc;
}

void curve2d::removePoint(unsigned int p) {
  if(p >= nPoints){
    userInterface::printError("point does not exist");
    return;
  }
  
  pntf* c = new pntf[--nPoints];
  
  int i = 0;
  for(; i < p; i++)
    c[i] = controlPoints[i];
  
  for(; i < nPoints; i++)
    c[i] = controlPoints[i-1];
  
  delete [] controlPoints;
  controlPoints = c;
}

bSpline::bSpline(unsigned int nPnts, pntf* cPnts, unsigned int order, float* r):
curve2d(nPnts, cPnts){
  k = order;
  u = r;
  
  if(order > nPnts)
    userInterface::printError("Too few Points for order");
}

bSpline::bSpline(unsigned int nPnts, pntf* cPnts, unsigned int order) {
  float* f = new float[nPnts + order + 1];
  
  for(int i = 0; i < nPnts + order + 1; i++)
    f[i] = (float)(i+1);
  
  bSpline(nPnts, cPnts, order, f);
}

unsigned int nChoosek( unsigned int n, unsigned int k )
{
  if (k > n) return 0;
  if (k * 2 > n) k = n-k;
  if (k == 0) return 1;

  int result = n;
  for( int i = 2; i <= k; ++i ) {
      result *= (n-i+1);
      result /= i;
  }

  return result;
}

pntf* bezier::draw(unsigned int resolution) {
  if(resolution < 2){
    userInterface::printError("need a resolution of at least 2");
    return nullptr;
  }
  
  if(getNumPoints() < 2){
    userInterface::printError("can't draw a curve with too few points");
    return nullptr;
  }
  
  pntf* p = new pntf[resolution];
  p[0] = getControlPoint(0);
  p[resolution-1] = getControlPoint(getNumPoints()-1);
  
  float dt = 1/(resolution-1);
  for(int i = 1; i < resolution-1; i++){
    p[i] = {0,0};
    float t = i*dt;
    
    for(int j = 0; j < getNumPoints(); j++)
      p[i] += getControlPoint(j) * (nChoosek(getNumPoints(), j) * pow(1-t,getNumPoints()-j) * pow(t,j));
    
  }
  
  return p;
}

pntf* bSpline::draw(unsigned int resolution) {
  if(resolution < 2){
    userInterface::printError("need a resolution of at least 2");
    return nullptr;
  }
  
  if(getNumPoints() < 2){
    userInterface::printError("can't draw a curve with too few points");
    return nullptr;
  }
  
  pntf* p = new pntf[resolution];
  float uMin = u[k-1];
  float uMax = u[getNumPoints()];
  
  float dt = (uMax - uMin) / (resolution - 1);
  
  for(int i = 0; i < resolution; i++){
    
    float pos = uMin + (i * dt);
    unsigned int seg = getSegment(pos);
    
    pntf d[k];
    
    std::copy(&(getControlPoints()[seg]), &(getControlPoints()[seg + k]), d);
    
    for(int j = 0; j < k; j++){
      for(int q = 0; q < k-j; q++){
        float foo = (u[q+k] - pos) / (u[q+k] - u[q+j]);
        float bar = (pos - u[q+j]) / (u[q+k] - u[q+j]);
        d[q] = (d[q] * foo) + (d[q+1] * bar);
      }
    }
    
    p[i] = d[0];
  }
  
  return p;
}

/*float bSpline::getU(unsigned int i) {
  float f = 0;
  
  for(int j = 0; j < i; j++)
    f += u[j];
  
  return f;
}*/

unsigned int bSpline::getSegment(float f) {
  unsigned int i = 0;
  
  while(u[i+k] < f) i++;
  
  return i;
}
