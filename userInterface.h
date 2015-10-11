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

  static void printError(string s);
  //inline static void selectObj(unsigned int i){objSelected = i;};
  
private:
  static void drawUI();
  static int objSelected;
};

#endif	/* USERINTERFACE_H */
