/* 
 * File:   userInterface.cpp
 * Author: btacklind
 * 
 * Created on October 8, 2015, 1:02 PM
 */

#include "userInterface.h"

#include <curses.h>
int userInterface::objSelected;

void userInterface::init(){
  initscr();
  //(void)echo();
  objSelected = -1;
  drawUI();
}

void userInterface::drawUI(){
  clear();
  printw("USE [ESC] TO END PROGRAM\n");
  printw("Current Object Selected: ");
  if (objSelected == -1) printw("None\n");
  else printw("%i\n", objSelected);
  printw("Current Action: ");

  refresh();
}

void userInterface::endUI(){
  endwin();
}

void userInterface::keypressed(unsigned char key){
  
}

