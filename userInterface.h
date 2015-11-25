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
#include "curve2d.h"
#include <sys/time.h>

using namespace std;

class userInterface {
public:
  enum DrawMode {curves, control, both};

  static void init();
  static void endUI();
  static void keypressed(unsigned char key);  
  static void leftMouseClick(int x, int y, bool buttonDown);
  static void mouseMove(int x, int y);
  static void mouseMovePassive(int x, int y);

  static void printError(char* s);
  static void holdUntilUsage();

  inline static bool drawCurves(){return dMode != control;};
  inline static bool drawControl(){return dMode != curves;};

  static char* getDrawStr();

  inline static void setIsMaxRes(bool b){isMaximum = b;};
  
  inline static curve2d* getSelectedCurve(){return selectedCurve;};
  
  static unsigned int getHelpHeight(){return helpHeight;};

  inline static bool isValueMode(){return valueMode;};
private:
  enum mode {Modify, Add, Remove, UMod};
  static mode currMode;
  
  static char* getModeStr();
  
  static const unsigned int helpHeight = 50;
  
  static char* action;
  
  static void drawUI();
  static int objSelected;

  static DrawMode dMode;
  
//  static bool onWindow;
  static bool isStarted;

  static bool isMaximum;

  static bool valueMode;
  static valueHolder* vals;
  
  static const unsigned int clickDistance = 10;
  
  static curve2d* selectedCurve;
  static unsigned int selectedPoint;
  
  static unsigned long int lastTime;
  static void setTime();
  static unsigned long int getTime();
  static const unsigned long int holdTime = 250;

  static bool isMoving;
  
  class interpretNewAddPoint: public InterpreterFunc{
  public:
    interpretNewAddPoint(curve2d* c, unsigned int i, char* m): curve(c), mes(m), index(i){};
    char* operator()(char**);
  private:
    curve2d* curve;
    const char* mes;
    const unsigned int index;
  };

  class interpretNewModPoint: public InterpreterFunc{
  public:
    interpretNewModPoint(curve2d* c, unsigned int i, char* m): curve(c), mes(m), index(i){};
    char* operator()(char**);
  private:
    curve2d* curve;
    const char* mes;
    const unsigned int index;
  };

  class interpretNewUModPoint: public InterpreterFunc{
  public:
    interpretNewUModPoint(bSpline* c, unsigned int i, char* m): curve(c), mes(m), index(i){};
    char* operator()(char**);
  private:
    bSpline* curve;
    const char* mes;
    const unsigned int index;
  };

  class interpretLoad: public InterpreterFunc{
  public:
    interpretLoad(){};
    char* operator()(char**);
  };
  
  class interpretSave: public InterpreterFunc{
  public:
    interpretSave(){};
    char* operator()(char**);
  };

  class interpretNewUInt: public InterpreterFunc{
  public:
    interpretNewUInt(void(*f)(unsigned int), char* m):setter(f), mes(m){};
    char* operator()(char**);
  private:
    void (*setter)(unsigned int);
    const char* mes;
  };
/*
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

  class interpretLightSize: public InterpreterFunc{
  public:
    interpretLightSize(){};
    char* operator()(char**);
  };*/
}; 



#endif	/* USERINTERFACE_H */

