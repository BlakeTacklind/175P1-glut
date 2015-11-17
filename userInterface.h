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
};

#endif	/* USERINTERFACE_H */

