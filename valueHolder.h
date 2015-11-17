/* 
 * File:   valueHolder.h
 * Author: btacklind
 *
 * Created on October 22, 2015, 2:25 PM
 */
#ifndef VALUEHOLDER_H
#define	VALUEHOLDER_H

#include <string>

using namespace std;

class valueHolder {
public:
  
  valueHolder();
  valueHolder(unsigned int i);
  valueHolder(const valueHolder& orig);
  virtual ~valueHolder();
  
  void addChar(unsigned char c);
  bool removeChar();
  inline bool isNext(){return nVal-1 > onVal;};
  bool nextVal();
  
  inline char* getMessage(){return message;};
  
private:
  char** strvals;
  const unsigned int nVal = 0;
  unsigned int onVal;
  unsigned int onChar;
  char* message;

  static const unsigned int maxCharLength = 20;
};

#endif	/* VALUEHOLDER_H */

