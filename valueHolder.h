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
  enum valTypes{
    None, ThreeFloats, SevenFloats, SingleInt, SingleString
  };
  
  valueHolder()=0;
  valueHolder(valTypes tp, unsigned int i);
  valueHolder(const valueHolder& orig);
  virtual ~valueHolder();
  
  void addChar(unsigned char c);
  bool removeChar();
  inline bool isNext(){return nVal-1 > onVal;};
  virtual bool nextVal() = 0;
  inline void* getVal(unsigned int i){return i<nVal?vals[i]:nullptr;};
  inline void** getValues(){return vals;};
  
  
protected:
  const valTypes t = None;
  void** vals;
  unsigned char** strvals;
  const unsigned int nVal = 0;
  const unsigned int maxCharLength = 20;
  unsigned int onVal;
  unsigned int onChar;
private:
};

class threeFloats: public valueHolder{
protected:
  const unsigned int numVal = 3;
public:
  threeFloats();
  bool nextVal();
};

class sevenFloats: public valueHolder{
protected:
  const unsigned int numVal = 7;
public:
  sevenFloats();
  bool nextVal();
};

class singleInt: public valueHolder{
protected:
  const unsigned int numVal = 1;
public:
  singleInt();
  bool nextVal();
};

class singleString: public valueHolder{
protected:
  const unsigned int numVal = 1;
public:
  singleString();
  bool nextVal();
};

#endif	/* VALUEHOLDER_H */

