/* 
 * File:   userInterface.cpp
 * Author: btacklind
 * 
 * This file handles the user interface
 */

#include "userInterface.h"
#include "OpenGLhandler.h"
#include <curses.h>

int userInterface::objSelected;
char* userInterface::action;
bool userInterface::onWindow;

void userInterface::init(){
  initscr();
  action = "";
  //(void)echo();
  objSelected = -1;
  onWindow = true;
  drawUI();
}

void userInterface::drawUI(){
  clear();
  printw("USE [ESC] TO END PROGRAM, press [h] for help");
  printw("\nHAVE FOCUS ON: %s\n", onWindow?"GLUT screen":"Terminal");
  printw("\nDraw Mode: %s", OpenGLhandler::getDrawMode());
  printw("\nAlgorithm Mode: %s", OpenGLhandler::getAlgMode());
  printw("\nCurrent Object Selected: ");
  if (objSelected == -1) printw("None");
  else printw("%i", objSelected);
  printw("\n%s", action);

  refresh();
}

void userInterface::printError(char* mes){
  action = mes;
  drawUI();
}

void userInterface::holdUntilUsage(){
  printw("Press any key to continue...");
  getch();
  refresh();
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

    onWindow = false;
    
    action = "Current Action: Enter translation values, x first\n";
    drawUI();
    
    char str[80];
    getstr(str);
    int val = atoi(str);
    
    getstr(str);
    
    onWindow = true;
    action = "";
    drawUI();
    
    object2D::getObject(objSelected)->translation(val, atoi(str));
    
    OpenGLhandler::bufferObjects();
    OpenGLhandler::reDraw();
  }
  else if(key == 'r'){
    if(objSelected == -1){
      printError("Error: No object selected");
      return;
    }
    onWindow = false;
    
    action = "Current Action: Enter rotation angle, in degrees\n";
    drawUI();
    
    char str[80];
    getstr(str);
    
    onWindow = true;
    action = "";
    drawUI();
    
    object2D::getObject(objSelected)->rotation(atof(str));
    
    OpenGLhandler::bufferObjects();
    OpenGLhandler::reDraw();
  }
  else if(key == 'e'){
    if(objSelected == -1){
      printError("Error: No object selected");
      return;
    }
    onWindow = false;
    
    action = "Current Action: Enter scaling value, x first\n";
    drawUI();
    
    char str[80];
    getstr(str);
    float val = atof(str);

    getstr(str);
    
    onWindow = true;
    action = "";
    drawUI();
    
    object2D::getObject(objSelected)->scale(val, atof(str));
    
    OpenGLhandler::bufferObjects();
    OpenGLhandler::reDraw();
  }
  else if(key == 'c'){
    action = "Current Action: Enter id of object to be selected\n";
    onWindow = false;
    drawUI();
    
    char str[80];
    getstr(str);
    
    action = "";
    onWindow = true;
    
    int val = atoi(str);
    
    if(val >= object2D::getNumObjects()){
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
    
    onWindow = false;
    char k = getch();
   if (k == '\033'){
   getch();
   k = getch();
    if(k == 'A'){
      action = "Current Action: enter value for Ymax\n";
      drawUI();
      
      char str[80];
      getstr(str);
      
      int val = atoi(str);
      onWindow = true;
      
      if(val < OpenGLhandler::getYmin()){
        printError("Ymax can't be less then Ymin");
        return;
      }
      if(val >= OpenGLhandler::getScreenHeight()){
        printError("Ymax can't be bigger then screen");
        return;
      }
      
      OpenGLhandler::setYmax(val);
    }
    else if(k == 'B'){
      action = "Current Action: enter value for Ymin\n";
      drawUI();
      
      char str[80];
      getstr(str);
      
      int val = atoi(str);
      onWindow = true;
      
      if(val > OpenGLhandler::getYmax()){
        printError("Ymin can't be greater then Ymax");
        return;
      }
      if(val < 0){
        printError("Ymin can't be less then zero");
        return;
      }
      
      OpenGLhandler::setYmin(val);
    }
    else if(k == 'D'){
      action = "Current Action: enter value for Xmin\n";
      drawUI();
      
      char str[80];
      getstr(str);
      
      int val = atoi(str);
      onWindow = true;
      
      if(val > OpenGLhandler::getXmax()){
        printError("Xmin can't be greater then Xmax");
        return;
      }
      if(val < 0){
        printError("Xmin can't be less then zero");
        return;
      }
      
      OpenGLhandler::setXmin(val);
    }
    else if(k == 'C'){
      action = "Current Action: enter value for Xmax\n";
      drawUI();
      
      char str[80];
      getstr(str);
      
      int val = atoi(str);
      
      onWindow = true;
      if(val < OpenGLhandler::getXmin()){
        printError("Xmax can't be less then Xmin");
        return;
      }
      if(val >= OpenGLhandler::getScreenWidth()){
        printError("Xmax can't be bigger then screen");
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
    printw("USE [ESC] TO END PROGRAM, press [h] for help");
    printw("\nHAVE FOCUS ON: %s\n", onWindow?"GLUT screen":"Terminal");
    printw("\nbracketed letter indicates which to press\n");
    printw("sele[c]t object mode\n");
    printw("toggle [a]lgorithm switch\n");
    printw("toggle [d]raw mode\n");
    printw("[t]ranslate selected object\n");
    printw("[r]otate selected object\n");
    printw("scal[e] selected object\n");
    printw("resize [v]iewport\n");
    printw("[s]ave to file\n");
    printw("[l]oad from file");

    refresh();
  }
  else if(key == 's'){
    action = "Current Action: save objects to file, leave blank for last file used\n";
    onWindow = false;
    drawUI();
    
    char str[80];
    getstr(str);

    if(str[0] == 0){if(object2D::save()) action = "File saved";}
    else if(object2D::save(str)) action = "File saved";
    
    onWindow = true;
    drawUI();
  }
  else if(key == 'l'){
    action = "Current Action: load objects from file, leave blank for last file used\n";
    onWindow = false;
    drawUI();
    
    char str[80];
    getstr(str);
    
    if(str[0] == 0){if(object2D::load()) action = "File Loaded";}
    else if(object2D::load(str)) action = "File Loaded";
    
    onWindow = true;
    drawUI();
    OpenGLhandler::bufferObjects();
    OpenGLhandler::reDraw();
  }
  else if(key == ' '){
    if( OpenGLhandler::getXmax() == 200)
      OpenGLhandler::setXmax(110);
    else OpenGLhandler::setXmax(200); 
    OpenGLhandler::bufferObjects();
    OpenGLhandler::reDraw();
  }
  
}

