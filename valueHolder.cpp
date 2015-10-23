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
}

valueHolder::valueHolder(const valueHolder& orig) {
}

valueHolder::~valueHolder() {
  while(!onVal)
    delete vals[--onVal];
  
  delete vals;
  delete strvals;
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
  vals    = new float*[numVal];
  strvals = new char[maxCharLength+1][numVal];
}

bool threeFloats::nextVal(){
  onChar = 0; 
  vals[onVal] = new float;
  *(vals[onVal]) = atof(strvals[onVal]);
  return nVal < ++onVal;
}

rotation::rotation():valueHolder(Rotation, numVal){
  vals    = new float*[numVal];
  strvals = new char[maxCharLength+1][numVal];
}

bool threeFloats::nextVal(){
  onChar = 0; 
  vals[onVal] = new float;
  *(vals[onVal]) = atof(strvals[onVal]);
  return nVal < ++onVal;
}

selection::selection():valueHolder(Selection, numVal){
  vals    = new float*[numVal];
  strvals = new char[maxCharLength+1][numVal];
}

bool selection::nextVal(){
  onChar = 0; 
  vals[onVal] = new int;
  *(vals[onVal]) = atoi(strvals[onVal]);
  return nVal < ++onVal;
}

singleString::singleString(valTypes tp):valueHolder(tp, numVal){
  vals    = new float*[numVal];
  strvals = new char[maxCharLength+1][numVal];
}

bool singleString::nextVal(){
  onChar = 0; 
  vals[onVal] = new char[maxCharLength+1];
  strcpy(vals[onVal], strvals[onVal]);
  return nVal < ++onVal;
}
