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
#include "userInterface.h"

using namespace std;

valueHolder::valueHolder(unsigned int in, char** del, char* def, InterpreterFunc* f):nVal(in){
  delimiters = del;
  defaultStr = def;
  interpreter = f;

  onVal = 0;
  onChar = 0;
  
  strvals = new char*[nVal];
  strvals[0] = new char[maxCharLength+1];
  strcpy(strvals[0], defaultStr);
}
valueHolder::valueHolder():nVal(0){
  onVal = 0;
  onChar = 0;
}

valueHolder::~valueHolder(){
  delete interpreter;
  delete [] delimiters;
}

valueHolder::valueHolder(const valueHolder& orig) {

}

void valueHolder::addChar(unsigned char c){
  if(maxCharLength<=onChar){
    userInterface::printError("String to long, remove characters or go to next value");
    return;
  }
  strvals[onVal][onChar++] = c;
  strvals[onVal][onChar  ] = 0;
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
  if (nVal == ++onVal) return false;

  strvals[onVal] = new char[maxCharLength+1];
  strcpy(strvals[onVal], defaultStr);
  
  return true;
}

char* valueHolder::getMessage(){
  char* str = new char[80];

  strcpy(str, delimiters[0]);

  int i = 0;
  for (; i <= onVal; i++){
    strcat(str, strvals[i]);
    strcat(str, delimiters[i+1]);
  }

  for (; i < nVal; i++){
    strcat(str, defaultStr);
    strcat(str, delimiters[i+1]);
  }

  return str;
}


pnt valueHolder::getCursorRelative(){
  unsigned int len = strlen(delimiters[0]);
  int i = 0;
  for (; i < onVal; i++){
    len += strlen(strvals[i]);
    len += strlen(delimiters[i+1]);
  }

  return {(int)(len + onChar), 0};
}

