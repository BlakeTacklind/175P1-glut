/* 
 * File:   userInterface.h
 * Author: btacklind
 *
 * Created on October 8, 2015, 1:02 PM
 */

#ifndef USERINTERFACE_H
#define	USERINTERFACE_H

#include <string>
using namespace std;

class userInterface {
public:
  static void init();
  static void endUI();
  static void keypressed(unsigned char key);  

  static void printError(char* s);
  static void holdUntilUsage();

  inline static bool isWindowFocus(){return onWindow;};

private:
  static char* action;
  
  static void drawUI();
  static int objSelected;

  //focus on main Window?
  static bool onWindow;
};

#endif	/* USERINTERFACE_H */

