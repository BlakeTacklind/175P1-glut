/* 
 * File:   curve2d.cpp
 * Author: btacklind
 * 
 * Created on November 21, 2015, 2:20 PM
 */

#include "curve2d.h"
#include "userInterface.h"
#include <math.h>
#include <list>
#include <fstream>
#include <iostream>

using namespace std;

char* curve2d::storedFileName;
unsigned int curve2d::nCurves;
curve2d** curve2d::curveList;

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
  myType = BSpline;
  k = order;
  u = r;
  
  if(order > nPnts)
    userInterface::printError("Too few Points for order");
}

bSpline::bSpline(unsigned int nPnts, pntf* cPnts, unsigned int order)
:bSpline(nPnts, cPnts, order, getDefaultKnots(order+nPnts)){
}

float* bSpline::getDefaultKnots(unsigned int j){
  float* f = new float[j];
  
  for(int i = 0; i < j; i++)
    f[i] = (float)(i+1);

  return f;
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
  
  float dt = 1.0/(resolution-1);
  for(int i = 1; i < resolution-1; i++){
    p[i] = {0,0};
    float t = i*dt;
    
    for(int j = 0; j < getNumPoints(); j++)
      p[i] += getControlPoint(j) * (nChoosek(getNumPoints()-1, j) * pow(1-t,getNumPoints()-j-1) * pow(t,j));
    
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
  float uMin = u[k-2];
  float uMax = u[getNumPoints()-1];
  
  float dt = (uMax - uMin) / (resolution - 1);
  
  for(int i = 0; i < resolution; i++){
    
    float pos = uMin + (i * dt);
    unsigned int seg = getSegment(pos);
    
    pntf d[k];
    
    std::copy(&(getControlPoints()[seg]), &(getControlPoints()[seg + k]), d);


    for(int j = 0; j < k-1; j++){
      for(int q = 0; q < k-j-1; q++){
        float uL = u[seg+q+j];
        float uR = u[k+seg-1+q];

        float foo = (uR - pos) / (uR - uL);
        float bar = (pos - uL) / (uR - uL);

        d[q] = (d[q] * foo) + (d[q+1] * bar);
      }
    }

    
    p[i] = d[0];
  }

  return p;
}

unsigned int bSpline::getSegment(float f) {
  unsigned int i = 0;
  
  while(u[i+k-1] < f) i++;
  
  return i;
}

bool curve2d::load(char* filename){

  ifstream file(filename, ios::in);
  

  if(file.is_open()){
    storedFileName = filename;
   
    list<curve2d*> tempList;
    string line;
    nCurves = 0;
    
    /*
     * Get the first line of the file
     * It should be number of objects.
     * If bad input close file and spit out error.
     */
    if(getline(file, line)){
      int numObjects = atoi(line.c_str());
      
      if (numObjects < 0){
        userInterface::printError("negative number of curves?!?!");
        return closeStuff(file, tempList, false);
      }
      else if (numObjects == 0){
        userInterface::printError("bad input or zero curves"); 
        return closeStuff(file, tempList, false);
      }
      
      nCurves = numObjects;
    }
    else{
      userInterface::printError("Empty file!");
      return closeStuff(file, tempList, false);
    }
    
    /*
     * iterate through the number of objects
     */
    
    for(int i = 0; i < nCurves; i++){
      /*
       * Get type of curve
       */
      curveType cType;
      if(getline(file, line)){
        if (!line.compare(0,2,"bz")){
          cType = Bezier;
        }
        else if (!line.compare(0,2,"bs")){
          cType = BSpline;
        }
        else{
          userInterface::printError("Expected a curve type");
          return closeStuff(file, tempList, false);
        }
      }
      else{
        userInterface::printError("Missing Curve definition");
        return closeStuff(file, tempList, false);
      }
      
      int numPoints;
      if(getline(file, line)){
        numPoints = atoi(line.c_str());

        if (numPoints < 0){
          userInterface::printError("negative points?!?!");
          return closeStuff(file, tempList, false);
        }
        else if (numPoints == 0){
          userInterface::printError("bad input or zero points");  
          return closeStuff(file, tempList, false);
        }
      }
      else{
        userInterface::printError("Missing Object definition");
        return closeStuff(file, tempList, false);
      }

      int k;
      if(cType == BSpline){
        if(getline(file, line)){
          k = atoi(line.c_str());

          if (k < 0){
            userInterface::printError("negative k in not possible");
            return closeStuff(file, tempList, false);
          }
          else if (k < 2){
            userInterface::printError("k must be at least 2");  
            return closeStuff(file, tempList, false);
          }

          if(k > numPoints){
            userInterface::printError("k cannot be greater then the number of points");  
            return closeStuff(file, tempList, false);
          }
        }
        else{
          userInterface::printError("Missing Object definition");
          return closeStuff(file, tempList, false);
        }
      }

      /*
       * Iterate through the number of control points
       */
      pntf* points = new pntf[numPoints];
      for (int j = 0; j < numPoints; j++){
        if(getline(file, line)){
          int del = line.find_first_of(' ');
          
          if (del < 1){
            userInterface::printError("Bad point delimiter");
            return closeStuff(file, tempList, false);
          }
          
          float x = atof(line.substr(0, del).c_str());
          float y = atof(line.substr(del+1).c_str());
          
          points[j].x = x;
          points[j].y = y;
        }
        else{
          userInterface::printError("Missing Point definition");
          return closeStuff(file, tempList, false);
        }
      }
      
      if(cType == BSpline){
        if(getline(file, line)){
          if (!line.compare("T")){
            float last = 0;
            float* knots = new float[numPoints+k];

            for(int j = 0; j < numPoints+k; j++){
              if(getline(file, line)){
                knots[j] = atof(line.c_str());

                if (knots[j] < last){
                  userInterface::printError("knot must be at a later point then last knot");
                  return closeStuff(file, tempList, false);
                }

                last = knots[j];
              }
              else{
                userInterface::printError("Missing Knot definition");
                return closeStuff(file, tempList, false);
              }
            }

            tempList.push_back(new bSpline(numPoints, points, k, knots));
          }
          else if (!line.compare("F")){
            tempList.push_back(new bSpline(numPoints, points, k));
          }
          else{
            userInterface::printError("Expected knot avability");
            return closeStuff(file, tempList, false);
          }
        }
        else{
          userInterface::printError("Missing knot avability");
          return closeStuff(file, tempList, false);
        }
      }
      else{
        tempList.push_back(new bezier(numPoints, points));
      }

    }
    
    return closeStuff(file, tempList, true);
  }
  userInterface::printError("Unable to open file");
  
  return false;
}

template <class E>
E* getArrFromList(list<E> l){
  E* arr = new E[l.size()];
  
  typename list<E>::iterator itr = l.begin();
  for(int i = 0; itr != l.end(); itr++, i++)
    arr[i] = *itr;
  
  return arr;
}

bool curve2d::closeStuff(ifstream& f, list<curve2d*>& tList, bool ret){
  curveList = getArrFromList(tList);
  nCurves = tList.size();
  
  f.close();
  return ret;
}

void curve2d::freeAll(){
  for(int i = 0; i < nCurves; i++)
    delete curveList[i];

  delete [] curveList;
}


pntf* curve2d::getMinMax(){
  pntf min, max;

  min = max = controlPoints[0];

  for(int i = 1; i < nPoints; i++){
    pntf p = controlPoints[i];

         if(p.x > max.x) max.x = p.x;
    else if(p.x < min.x) min.x = p.x;

         if(p.y > max.y) max.y = p.y;
    else if(p.y < min.y) min.y = p.y;
  }

  pntf* p = new pntf[2];

  p[0] = min;
  p[1] = max;

  return p;
}

bool curve2d::save(char* filename){
  
  ofstream file(filename);
  if (file.is_open()){
    storedFileName = filename;
    file << nCurves;
    for (int i = 0; i < nCurves; i++){

      if(curveList[i]->myType == BSpline) file<<"\nbs";
      else file<<"\nbz";

      file << "\n" << curveList[i]->nPoints;

      if(curveList[i]->myType == BSpline) file<<"\n"<<((bSpline*)curveList[i])->getK();

      for (int j = 0 ; j < curveList[i]->nPoints; j++)
        file << "\n" << curveList[i]->controlPoints[j].x << " " << curveList[i]->controlPoints[j].y;
      
      if(curveList[i]->myType == BSpline){
        file<<"\nT";
        for (int j = 0 ; j < curveList[i]->getNumPoints() + ((bSpline*)curveList[i])->getK(); j++)
          file << "\n"<< ((bSpline*)curveList[i])->getU(j);
      }

    }
    
    file.close();
    return true;
  }
  userInterface::printError("failed to open save file");
  return false;
}
