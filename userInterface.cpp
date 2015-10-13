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
char* userInterface::action;

void userInterface::init(){
  initscr();
  message = "";
  action = "";
  //(void)echo();
  objSelected = -1;
  drawUI();
}

void userInterface::drawUI(){
  clear();
  printw("USE [ESC] TO END PROGRAM, press [h] for help\n");
  printw("\nDraw Mode: %s", OpenGLhandler::getDrawMode());
  printw("\nAlgorithm Mode: %s", OpenGLhandler::getAlgMode());
  printw("\nCurrent Object Selected: ");
  if (objSelected == -1) printw("None");
  else printw("%i", objSelected);
  printw("\nCurrent Action: %s", action);
  printw("\n%s", message);

  refresh();
}

void userInterface::printError(char* mes){
  action = "";
  message = mes;
  drawUI();
}

void userInterface::endUI(){
  endwin();
}


void userInterface::keypressed(unsigned char key){
  if(key == 'd'){
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
      return;
    }
    
    action = "Enter translation values, x first";
    drawUI();
    
    char str[80];
    getstr(str);
    int val = atoi(str);
    
    getstr(str);
    
    action = "";
    drawUI();
    
    obj::getObject(objSelected).translation(val, atoi(str));
    
    OpenGLhandler::bufferObjects();
    OpenGLhandler::reDraw();
  }
  else if(key == 'r'){
    if(objSelected == -1){
      printError("Error: No object selected");
      return;
    }
    
    action = "Enter rotation angle, in degrees";
    drawUI();
    
    char str[80];
    getstr(str);
    
    action = "";
    drawUI();
    
    obj::getObject(objSelected).rotation(atof(str));
    
    OpenGLhandler::bufferObjects();
    OpenGLhandler::reDraw();
  }
  else if(key == 'e'){
    if(objSelected == -1){
      printError("Error: No object selected");
      return;
    }
    
    action = "Enter scaling value, x first";
    drawUI();
    
    char str[80];
    getstr(str);
    float val = atof(str);

    getstr(str);
    
    action = "";
    drawUI();
    
    obj::getObject(objSelected).scale(val, atof(str));
    
    OpenGLhandler::bufferObjects();
    OpenGLhandler::reDraw();
  }
  else if(key == 'c'){
    action = "Enter id of object to be selected";
    drawUI();
    
    char str[80];
    getstr(str);
    
    action = "";
    
    int val = atoi(str);
    
    if(val >= obj::getNumObjects()){
      printError("Not an object id!");
      return;
    }
    
    objSelected = val;
    
    drawUI();
  }
  else if(key == 'v'){
    action = "Resize viewport: choose which edge to resize\n"
            "use arrows keys to determine which side";
    drawUI();
    
    char k = getch();
   if (k == '\033'){
   getch();
   k = getch();
    if(k == 'A'){
      action = "enter value for Ymax";
      drawUI();
      
      char str[80];
      getstr(str);
      
      int val = atoi(str);
      
      if(val < OpenGLhandler::getYmin()){
        printError("Ymax can't be less then Ymin");
        return;
      }
      
      OpenGLhandler::setYmax(val);
    }
    else if(k == 'B'){
      action = "enter value for Ymin";
      drawUI();
      
      char str[80];
      getstr(str);
      
      int val = atoi(str);
      
      if(val > OpenGLhandler::getYmax()){
        printError("Ymin can't be greater then Ymax");
        return;
      }
      
      OpenGLhandler::setYmin(val);
    }
    else if(k == 'D'){
      action = "enter value for Xmin";
      drawUI();
      
      char str[80];
      getstr(str);
      
      int val = atoi(str);
      
      if(val > OpenGLhandler::getXmax()){
        printError("Xmin can't be greater then Xmax");
        return;
      }
      
      OpenGLhandler::setXmin(val);
    }
    else if(k == 'C'){
      action = "enter value for Xmax";
      drawUI();
      
      char str[80];
      getstr(str);
      
      int val = atoi(str);
      
      if(val < OpenGLhandler::getXmin()){
        printError("Xmax can't be less then Xmin");
        return;
      }
      
      OpenGLhandler::setXmax(val);
    }
    }
    action = "";
    drawUI();
    OpenGLhandler::bufferObjects();
    OpenGLhandler::reDraw();
  }
  else if(key == 'h'){
    clear();
    printw("USE [ESC] TO END PROGRAM, press [h] for help\n");
    printw("bracketed letter indicates which to press");
    printw("sele[c]t object mode\n");
    printw("toggle [a]lgorithm switch\n");
    printw("toggle [d]raw mode\n");
    printw("[t]ranslate selected object\n");
    printw("[r]otate selected object\n");
    printw("scal[e] selected object\n");
    printw("resize [v]iewport\n");
    printw("[s]ave to file");
    printw("[l]oad from file");

    refresh();
  }
  else if(key == 's'){
    action = "save objects to file, leave blank for last file used";
    drawUI();
    
    char str[80];
    getstr(str);
    
    if(str == "") obj::save();
    else obj::save(str);
    
    message = "file saved!";
    action = "";
    drawUI();
  }
  else if(key == 'l'){
    action = "load objects from file, leave blank for last file used";
    drawUI();
    
    char str[80];
    getstr(str);
    
    if(str == "") obj::load();
    else obj::load(str);
    
    message = "file loaded!";
    action = "";
    drawUI();
    OpenGLhandler::bufferObjects();
    OpenGLhandler::reDraw();
  }
  
}

