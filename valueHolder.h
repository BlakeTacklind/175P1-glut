/* 
 * File:   valueHolder.h
 * Author: btacklind
 *
 * Created on October 22, 2015, 2:25 PM
 */
#ifndef VALUEHOLDER_H
#define	VALUEHOLDER_H

#include <string>
#include "InterpreterFunc.h"
#include "types.h"

using namespace std;

class valueHolder {
public:
  
  valueHolder();
  valueHolder(unsigned int i, char** del, char* def, InterpreterFunc* f);
  valueHolder(const valueHolder& orig);
  virtual ~valueHolder();
  
  void addChar(unsigned char c);
  bool removeChar();
  inline bool isNext(){return nVal-1 > onVal;};
  bool nextVal();

  char* getValue(unsigned int i){return strvals[i];}
  
  char* getMessage();
  inline char* interpret(){return (*interpreter)(strvals);};

  pnt getCursorRelative();

private:
  char** strvals;
  char** delimiters;
  const unsigned int nVal = 0;
  unsigned int onVal;
  unsigned int onChar;
  char* defaultStr;

  InterpreterFunc* interpreter;

  static const unsigned int maxCharLength = 20;
};

#endif	/* VALUEHOLDER_H */

