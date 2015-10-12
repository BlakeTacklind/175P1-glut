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
char* userInterface::message;
int userInterface::val1;
int userInterface::val2;
bool userInterface::enteringValue;

void userInterface::init(){
  initscr();
  message = "";
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
  printw("\n%s", message);

  refresh();
}

void userInterface::printError(char* mes){
  message = mes;
}

void userInterface::endUI(){
  endwin();
}



void userInterface::keypressed(unsigned char key){
  if(enteringValue){
    
  }
  else if(key == 'd'){
    OpenGLhandler::tglDrawMode();
    OpenGLhandler::bufferObjects();
    OpenGLhandler::reDraw();
    drawUI();
  }
  else if (key == 'a'){
    OpenGLhandler::tglAlgMode();
    OpenGLhandler::bufferObjects();
    OpenGLhandler::reDraw();
    drawUI();
  }
  else if (key == 't'){
    if(objSelected == -1){
      printError("Error: No object selected");
    }
    
    enteringValue = true;
  }
}

