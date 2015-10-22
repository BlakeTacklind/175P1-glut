/* 
 * File:   userInterface.cpp
 * Author: btacklind
 * 
 * This file handles the user interface
 */

#include "userInterface.h"
#include "OpenGLhandler.h"
#include "object3D.h"
#include "screen.h"
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
  if (key == 't'){
    if(objSelected == -1){
      printError("Error: No object selected");
      return;
    }

    onWindow = false;
    
    action = "Current Action: Enter translation values, x first\n";
    drawUI();
    printw("Translate %i (0  , 0  , 0  )", objSelected);
    move(5, 13);
    
    char str[80];
    getstr(str);
    int val = atof(str);
    
    getstr(str);
    int val2 = atof(str);

    getstr(str);
    
    onWindow = true;
    action = "";
    drawUI();
    
    object3D::getObject(objSelected)->translate(val, val2, atoi(str));
    
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
    
    pnt3 p1;
    pnt3 p2;

    char str[80];
    getstr(str);

    p1.x = atof(str);

    getstr(str);

    p1.y = atof(str);

    getstr(str);

    p1.z = atof(str);

    getstr(str);

    p2.x = atof(str);

    getstr(str);

    p2.y = atof(str);

    getstr(str);

    p2.z = atof(str);

    getstr(str);

    onWindow = true;
    action = "";
    
    if(p1 == p2){
      printError("Line is not defined! (points are the same)");
      return;
    }

    drawUI();
    object3D::getObject(objSelected)->rotate(p1, p2, atof(str));
    
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
    float val2 = atof(str);

    getstr(str);
    
    onWindow = true;
    action = "";
    drawUI();
    
    object3D::getObject(objSelected)->scale(val, val2, atof(str));
    
    OpenGLhandler::bufferObjects();
    OpenGLhandler::reDraw();
  }
  else if(key == 'c'){
    action = "Current Action: Enter id of object to be selected\n";
    onWindow = false;
    drawUI();
    printw("Select object: 0");
    move(5, 15);


    char str[80];
    getstr(str);
    
    action = "";
    onWindow = true;
    
    int val = atoi(str);
    
    if(val >= object3D::getNumObjects()){
      printError("Not an object id!");
      return;
    }
    
    objSelected = val;
    
    drawUI();
  }
  else if(key == 'h'){
    clear();
    printw("USE [ESC] TO END PROGRAM, press [h] for help");
    printw("\nHAVE FOCUS ON: %s\n", onWindow?"GLUT screen":"Terminal");
    printw("\nbracketed letter indicates which to press\n");
    printw("sele[c]t object mode\n");
    printw("[t]ranslate selected object\n");
    printw("[r]otate selected object\n");
    printw("scal[e] selected object\n");
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

    if(str[0] == 0){if(object3D::save()) action = "File saved";}
    else if(object3D::save(str)) action = "File saved";
    
    onWindow = true;
    drawUI();
  }
  else if(key == 'l'){
    action = "Current Action: load objects from file, leave blank for last file used\n";
    onWindow = false;
    drawUI();
    
    char str[80];
    getstr(str);
    
    if(str[0] == 0){if(object3D::load()) action = "File Loaded";}
    else if(object3D::load(str)) action = "File Loaded";
    
    onWindow = true;
    drawUI();
    OpenGLhandler::bufferObjects();
    OpenGLhandler::reDraw();
  }
  else if(key == ' '){
    screen* s = screen::getLastScreen();
    s->setNormal(rotateAboutZ(s->getNormal(), 1));

    OpenGLhandler::bufferObjects();
    OpenGLhandler::reDraw();
  }
  
}

