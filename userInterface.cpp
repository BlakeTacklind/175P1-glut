/* 
 * File:   userInterface.cpp
 * Author: btacklind
 * 
 * Created on October 8, 2015, 1:02 PM
 */

#include "userInterface.h"
#include "OpenGLhandler.h"
//#include <string>

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
  printw("\nDraw Mode: %s", OpenGLhandler::getDrawMode());
  printw("\nAlgorithm Mode: %s", OpenGLhandler::getAlgMode());
  printw("\nCurrent Object Selected: ");
  if (objSelected == -1) printw("None");
  else printw("%i", objSelected);
  printw("\nCurrent Action: ");

  refresh();
}

void userInterface::endUI(){
  endwin();
}

void userInterface::keypressed(unsigned char key){
  if(key == 'd'){
    OpenGLhandler::tglDrawMode();
    //OpenGLhandler::bufferObjects();
    OpenGLhandler::reDraw();
    drawUI();
  }
  else if (key == 'a'){
    OpenGLhandler::tglAlgMode();
    OpenGLhandler::bufferObjects();
    OpenGLhandler::reDraw();
    drawUI();
  }
}

