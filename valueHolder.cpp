/* 
 * File:   valueHolder.cpp
 * Author: btacklind
 * 
 * Created on October 22, 2015, 2:25 PM
 */

#include "valueHolder.h"
#include <cstdlib>
#include <cstring>
#include <string>

using namespace std;

valueHolder::valueHolder(valTypes tp, unsigned int in):t(tp){
  onVal = 0;
  onChar = 0;
  nVal = in;
  message = (char*)((string)("Entering a value ")).append(to_string(in)).append("\n").c_str();
}

valueHolder::~valueHolder(){
}

valueHolder::valueHolder(const valueHolder& orig) {
}

void valueHolder::addChar(unsigned char c){
  strvals[onVal][onChar++] = c;
  strvals[onVal][onChar  ] = 0;
  if(maxCharLength<onChar) nextVal();
}
/*
bool valueHolder::removeChar(){
  if(!onChar){
    if(onVal){
      delete vals[onVal];
      onChar = strlen(strvals[--onVal]);
      return false;
    }
    
    return true;
  }
  
  strvals[onVal][--onChar] = 0;
  
  return true;
}
*/
threeFloats::threeFloats(valTypes tp):valueHolder(tp, 3){
  vals    = (void**)new float*[maxNumVal];
  strvals = new char*[maxNumVal];
  strvals[0] = new char[maxCharLength+1];
}
/*
threeFloats::~threeFloats() {
  while(!onVal){
    delete (char*)strvals[onVal];
    delete (float*)vals[--onVal];
  }
  delete (char*) strvals[0];
  
  delete [] vals;
  delete [] strvals;
}
*/
bool threeFloats::nextVal(){
  onChar = 0; 
  *((float*)vals[onVal]) = atof(strvals[onVal]);
  
  if (nVal < ++onVal) return true;

  vals[onVal] = (void*)new float;
  return false;
}

rotation::rotation():valueHolder(Rotation, 7){
  vals    = (void**)new float*[maxNumVal];
  strvals = new char*[maxNumVal];
  strvals[0] = new char[maxCharLength+1];
}
/*
rotation::~rotation() {
  while(!onVal){
    delete (char*)strvals[onVal];
    delete (float*)vals[--onVal];
  }
  delete (char*) strvals[0];
  
  delete [] vals;
  delete [] strvals;
}
*/
bool rotation::nextVal(){
  onChar = 0;
  *((float*)vals[onVal]) = atof(strvals[onVal]);
  if (nVal < ++onVal) return true;
  
  vals[onVal] = (void*)new float;
  return false;
}

selection::selection():valueHolder(Selection, 1){
  vals    = (void**)new int*[maxNumVal];
  strvals = new char*[maxNumVal];
  strvals[0] = new char[maxCharLength+1];
}
/*
selection::~selection() {
  while(!onVal){
    delete (char*)strvals[onVal];
    delete (int*)  vals[--onVal];
  }
  delete (char*) strvals[0];
  
  delete [] vals;
  delete [] strvals;
}
*/
bool selection::nextVal(){
  onChar = 0;
  *((int*)vals[onVal]) = atoi(strvals[onVal]);

  if (nVal < ++onVal) return true;
  
  vals[onVal] = (void*)new int;
  return false;
}

singleString::singleString(valTypes tp):valueHolder(tp, 1){
  vals    = (void**)new char*[maxNumVal];
  strvals = new char*[maxNumVal];
  strvals[0] = new char[maxCharLength+1];
}
/*
singleString::~singleString() {
  while(!onVal){
    delete (char*)strvals[onVal];
    delete (char*) vals[--onVal];
  }
  delete (char*) strvals[0];
  
  delete [] vals;
  delete [] strvals;
}
*/
bool singleString::nextVal(){
  onChar = 0;
  strcpy((char*)vals[onVal], strvals[onVal]);
  vals[++onVal] = (void*)new char[maxCharLength+1];
  return nVal < onVal;
}
