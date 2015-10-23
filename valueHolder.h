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
    None, Translation, Scale, Rotation, Selection, Save, Load
  };
  
  valueHolder();
  valueHolder(valTypes tp, unsigned int i);
  valueHolder(const valueHolder& orig);
  virtual ~valueHolder() = 0;
  
  void addChar(unsigned char c);
  bool removeChar();
  inline bool isNext(){return nVal-1 > onVal;};
  virtual bool nextVal() = 0;
  inline void* getVal(unsigned int i){return i<nVal?vals[i]:nullptr;};
  inline void** getValues(){return vals;};
  
  inline valTypes getType(){return t;};
  
  inline char* getMessage(){return message;};
  
protected:
  const valTypes t = None;
  void** vals;
  char** strvals;
  const unsigned int nVal = 0;
  const unsigned int maxCharLength = 20;
  unsigned int onVal;
  unsigned int onChar;
  char* message;
private:
};

class threeFloats: public valueHolder{
protected:
  const unsigned int numVal = 3;
public:
  threeFloats(valTypes tp);
  virtual ~threeFloats();
  bool nextVal();
};

class translation: public threeFloats{
public:
  inline translation():threeFloats(Translation){};
};

class scale: public threeFloats{
public:
  inline scale():threeFloats(Scale){};
};

class rotation: public valueHolder{
protected:
  const unsigned int numVal = 7;
public:
  rotation();
  virtual ~rotation();
  bool nextVal();
};

class selection: public valueHolder{
protected:
  const unsigned int numVal = 1;
public:
  selection();
  virtual ~selection();
  bool nextVal();
};

class singleString: public valueHolder{
protected:
  const unsigned int numVal = 1;
public:
  singleString(valTypes tp);
  virtual ~singleString();
  bool nextVal();
};

class saveFile: public singleString{
public:
  inline saveFile():singleString(Save){};
};

class loadFile: public singleString{
public:
  inline loadFile():singleString(Load){};
};

#endif	/* VALUEHOLDER_H */

