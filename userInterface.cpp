/* 
 * File:   userInterface.cpp
 * Author: btacklind
 * 
 * Created on October 8, 2015, 1:02 PM
 */

#include "userInterface.h"

#include <curses.h>

void userInterface::init(){
  initscr();
  (void)echo();
}

