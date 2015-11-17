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

valueHolder::valueHolder(unsigned int in):nVal(in){
  onVal = 0;
  onChar = 0;
}
valueHolder::valueHolder():nVal(0){
  onVal = 0;
  onChar = 0;
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

bool valueHolder::removeChar(){
  if(!onChar){
    if(onVal){
      onChar = strlen(strvals[--onVal]);
      return false;
    }
    
    return true;
  }
  
  strvals[onVal][--onChar] = 0;
  
  return true;
}

bool valueHolder::nextVal(){
  onChar = 0; 
  
  if (nVal < ++onVal) return true;

  return false;
}
