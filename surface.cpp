/* 
 * File:   surface.cpp
 * Author: btacklind
 * 
 * Created on November 5, 2015, 2:29 PM
 */

#include "surface.h"
#include <string>

using namespace std;

surface::surface(unsigned int nPoints, int* pointNumbers, pnt3 normal){
  nPnts = nPoints;
  norm = normal;
  pntNums = pointNumbers;
  parent = nullptr;
}

surface::surface() {
}

surface::surface(const surface& orig) {
}

surface::~surface() {
}

bool surface::compare(const surface& first, const surface& second) {
  
}


const char* surface::getPntString() {
  string str = to_string(pntNums[0]);
  
  for(int i = 1; i < nPnts; i++){
    str.append(" ");
    str.append(to_string(pntNums[i]));
  }
  
  return str.c_str();
}

const char* surface::getNormalString(){
  string str = to_string(norm.x);
  str.append(" ");
  str.append(to_string(norm.y));
  str.append(" ");
  str.append(to_string(norm.z));
  
  return str.c_str();
}
