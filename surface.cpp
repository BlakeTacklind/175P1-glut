/* 
 * File:   surface.cpp
 * Author: btacklind
 * 
 * Created on November 5, 2015, 2:29 PM
 */

#include "surface.h"
#include <string>

using namespace std;

surface::surface(unsigned int nPoints, pnt3* points, pnt3 normal){
  pnts = points;
  nPnts = nPoints;
  norm = normal;
  pntNormals = nullptr;
}

surface::surface() {
}

surface::surface(const surface& orig) {
}

surface::~surface() {
}

char* surface::getPntString() {
  string str = to_string(pnts[0]);
  
  for(int i = 1; i < nPnts; i++){
    str.append(to_string(" "));
    str.append(to_string(pnts[i]));
  }
  
  return str.c_str();
}

char* surface::getNormalString(){
  string str = to_string(norm.x);
  str.append(to_string(" "));
  str.append(to_string(norm.y));
  str.append(to_string(" "));
  str.append(to_string(norm.z));
  
  return str.c_str();
}
