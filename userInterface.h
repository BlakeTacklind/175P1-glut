/* 
 * File:   userInterface.h
 * Author: btacklind
 *
 * Created on October 8, 2015, 1:02 PM
 */

#ifndef USERINTERFACE_H
#define	USERINTERFACE_H

#include <string>
#include "valueHolder.h"
#include "types.h"
#include "InterpreterFunc.h"

using namespace std;

class userInterface {
public:
  static void init();
  static void endUI();
  static void keypressed(unsigned char key);  

  static void printError(char* s);
  static void holdUntilUsage();

  inline static bool isValueMode(){return valueMode;};
private:
  static char* action;

  static void doAction();
  
  static void drawUI();
  static int objSelected;
  
  static bool onWindow;
  static bool isStarted;

  static bool valueMode;
  static valueHolder* vals;

  class interpretNewPnt3: public InterpreterFunc{
  public:
    interpretNewPnt3(void(*f)(pnt3), char* m):setter(f), mes(m){};
    char* operator()(char**);
  private:
    void (*setter)(pnt3);
    const char* mes;
  };

  class interpretNewFloat: public InterpreterFunc{
  public:
    interpretNewFloat(void(*f)(float), char* m):setter(f), mes(m){};
    char* operator()(char**);
  private:
    void (*setter)(float);
    const char* mes;
  };

  class interpretNewNormal: public InterpreterFunc{
  public:
    interpretNewNormal(){};
    char* operator()(char**);
  };

  class interpretLoad: public InterpreterFunc{
  public:
    interpretLoad(){};
    char* operator()(char**);
  };

  class interpretLightSize: public InterpreterFunc{
  public:
    interpretLightSize(){};
    char* operator()(char**);
  };
};



#endif	/* USERINTERFACE_H */

