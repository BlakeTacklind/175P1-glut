/* 
 * File:   valueHolder.cpp
 * Author: btacklind
 * 
 * Created on October 22, 2015, 2:25 PM
 */

#include "valueHolder.h"
#include <cstdlib>
#include <cstring>

valueHolder::valueHolder(valTypes tp, unsigned int i):t(tp), nVal(i){
  onVal = 0;
  onChar = 0;
  message = "Entering a value\n";
}

valueHolder::~valueHolder() {
}

valueHolder::valueHolder(const valueHolder& orig) {
}

void valueHolder::addChar(unsigned char c){
  strvals[onVal][onChar++] = c;
  strvals[onVal][onChar  ] = 0;
  if(maxCharLength<onChar) nextVal();
}

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

threeFloats::threeFloats(valTypes tp):valueHolder(tp, numVal){
  vals    = (void**)new float*[numVal];
  strvals = new char*[numVal];
  strvals[0] = new char[maxCharLength+1];
}

threeFloats::~threeFloats() {
  while(!onVal){
    delete (char*)strvals[onVal];
    delete (float*)vals[--onVal];
  }
  delete (char*) strvals[0];
  
  delete [] vals;
  delete [] strvals;
}

bool threeFloats::nextVal(){
  onChar = 0; 
  vals[onVal] = new float;
  *((float*)vals[onVal]) = atof(strvals[onVal]);
  return nVal < ++onVal;
}

rotation::rotation():valueHolder(Rotation, numVal){
  vals    = (void**)new float*[numVal];
  strvals = new char*[numVal];
  strvals[0] = new char[maxCharLength+1];
}

rotation::~rotation() {
  while(!onVal){
    delete (char*)strvals[onVal];
    delete (float*)vals[--onVal];
  }
  delete (char*) strvals[0];
  
  delete [] vals;
  delete [] strvals;
}

bool rotation::nextVal(){
  onChar = 0; 
  vals[onVal] = new float;
  *((float*)vals[onVal]) = atof(strvals[onVal]);
  return nVal < ++onVal;
}

selection::selection():valueHolder(Selection, numVal){
  vals    = (void**)new float*[numVal];
  strvals = new char*[numVal];
  strvals[0] = new char[maxCharLength+1];
}

selection::~selection() {
  while(!onVal){
    delete (char*)strvals[onVal];
    delete (int*)  vals[--onVal];
  }
  delete (char*) strvals[0];
  
  delete [] vals;
  delete [] strvals;
}

bool selection::nextVal(){
  onChar = 0; 
  vals[onVal] = new int;
  *((int*)vals[onVal]) = atoi(strvals[onVal]);
  return nVal < ++onVal;
}

singleString::singleString(valTypes tp):valueHolder(tp, numVal){
  vals    = (void**)new float*[numVal];
  strvals = new char*[numVal];
  strvals[0] = new char[maxCharLength+1];
}

singleString::~singleString() {
  while(!onVal){
    delete (char*)strvals[onVal];
    delete (char*) vals[--onVal];
  }
  delete (char*) strvals[0];
  
  delete [] vals;
  delete [] strvals;
}

bool singleString::nextVal(){
  onChar = 0;
  vals[onVal] = new char[maxCharLength+1];
  strcpy((char*)vals[onVal], strvals[onVal]);
  return nVal < ++onVal;
}
