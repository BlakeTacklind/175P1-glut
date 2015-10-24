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
#include <string>
#include <curses.h>

int userInterface::objSelected;
char* userInterface::action;
bool userInterface::valueMode;
bool userInterface::onWindow;
valueHolder* userInterface::vals;

void userInterface::init(){
  initscr();
  start_color();
  init_pair(1, COLOR_RED, COLOR_BLACK);
  action = "";
  if(!has_colors()) action = "Does not support color!";
  else if(!can_change_color()) action = "Can't change color";
  objSelected = 0;
  onWindow = true;
  drawUI();
}

void userInterface::drawUI(){
  clear();
  printw("USE [ESC] TO END PROGRAM, press [h] for help");
  attron(COLOR_PAIR(1));
  printw("\nHAVE FOCUS ON: %s\n", onWindow?"GLUT screen":"Terminal");
  attroff(COLOR_PAIR(1));
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

void userInterface::doAction(){
  if(vals->getType() == valueHolder::Translation){
      object3D::getObject(objSelected)->translate(*((float*)vals->getVal(0)), 
              *((float*)vals->getVal(1)), *((float*)vals->getVal(2)));
      OpenGLhandler::bufferObjects();
      OpenGLhandler::reDraw();
      return;
  }
  if(vals->getType() == valueHolder::Scale){ 
      object3D::getObject(objSelected)->scale(*((float*)vals->getVal(0)), 
              *((float*)vals->getVal(1)), *((float*)vals->getVal(2)));
      OpenGLhandler::bufferObjects();
      OpenGLhandler::reDraw();
      return;
  }
  if(vals->getType() == valueHolder::Rotation){
      pnt3 p1 = {*((float*)vals->getVal(0)), *((float*)vals->getVal(1)), *((float*)vals->getVal(2))};
      pnt3 p2 = {*((float*)vals->getVal(3)), *((float*)vals->getVal(4)), *((float*)vals->getVal(5))};
      object3D::getObject(objSelected)->rotate(p1, p2, *((float*)vals->getVal(6)));
      return;
  }
  if(vals->getType() == valueHolder::Selection){
      objSelected = *((int*)vals->getVal(0));
      return;
  }
  if(vals->getType() == valueHolder::Save){
      object3D::save((char*)vals->getVal(0));
      return;
  }
  if(vals->getType() == valueHolder::Load){
      object3D::load((char*)vals->getVal(0));
      OpenGLhandler::bufferObjects();
      OpenGLhandler::reDraw();
      return;
  }
}

void userInterface::keypressed(unsigned char key){
  /*if (valueMode){
    if(key == 9 || key == ' ' || key == 13){


      if(!vals->nextVal()){
        valueMode = false;
   
        doAction();
        
        action = "Value Entered!";
        drawUI();

        return;
      }

      action = "Finished Value!";

      drawUI();
   
      
      return;
    }/*
    if(key == 27){
      valueMode = false;
      delete vals;
      
      action = "";
      drawUI();
      return;
    }*/
    /*
    if(key > 31 && key < 127){

      vals->addChar(key);
    
      action = vals->getMessage();
      drawUI();
    }
  }
  else */if (key == 't'){
    if(objSelected == -1){
      printError("Error: No object selected");
      return;
    }

    onWindow = false;
    
    action = "Current Action: Enter translation values\n"
    "                   translate x\n"
    "                   translate y\n"
    "                   translate z\n";
    drawUI();
    move(5,0);
    /*
    printw("Translate %i (0  , 0  , 0  )", objSelected);
    move(5, 13);
    
    vals = new translation();
    action = vals->getMessage();
    drawUI();
    return;
    */
    char str[80];
    getstr(str);
    float val = atof(str);
    
    getstr(str);
    float val2 = atof(str);

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
    
    action = "Define an axis then a rotation angle, in degrees\n"
      "                       point 1 x\n"
      "                       point 1 y\n"
      "                       point 1 z\n"
      "                       point 2 x\n"
      "                       point 2 y\n"
      "                       point 2 z\n"
      "                       angle\n";
    /*
    vals = new rotation();
    action = vals->getMessage();
    drawUI();
    return;
    */
    drawUI();
    move(5,0);

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

    action = "";
    
    onWindow = true;
    
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
    
    action = "Current Action: Enter scale values\n"
    "                  scale x\n"
    "                  scale y\n"
    "                  scale z\n";
    /*
    vals = new scale();
    action = vals->getMessage();
    drawUI();
    return;*/
    drawUI();
    move(5,0);
    
    char str[80];
    getstr(str);
    float val = atof(str);

    getstr(str);
    float val2 = atof(str);

    getstr(str);
    
    action = "";
    onWindow = true;
    
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

    /*
    vals = new selection();
    action = vals->getMessage();
    drawUI();
    return;
    */
    char str[80];
    getstr(str);
    
    action = "";
    
    int val = atoi(str);
    
    onWindow = true;
    
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
    printw("[l]oad from file\n");
    printw("[space] for rotate oblique window about z-axis\n");
    printw("Set [n]ormal for oblique window\n");

    refresh();
  }
  else if(key == 's'){
    action = (char*)((string)("Save objects to file, hit return for ")).append(object3D::getStoredFile()).append("\n").c_str();
    drawUI();

    onWindow = false;
    
    /*
    vals = new saveFile();
    action = vals->getMessage();
    drawUI();
    return;
    */
    char str[80];
    getstr(str);
    onWindow = true;
    
    if(str[0] == 0){if(object3D::save()) action = "File saved";}
    else if(object3D::save(str)) action = "File saved";
    
    drawUI();
  }
  else if(key == 'l'){
    action = (char*)((string)("Load objects from file, hit return for ")).append(object3D::getStoredFile()).append("\n").c_str();
    drawUI();

    onWindow = false;
    
    /*
    vals = new loadFile();
    action = vals->getMessage();
    drawUI();
    return;
    */
    char str[80];
    getstr(str);
    onWindow = true;
    
    if(str[0] == 0){if(object3D::load()) action = "File Loaded";}
    else if(object3D::load(str)) action = "File Loaded";
    
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
  else if(key == 'n'){

    onWindow = false;
    
    action = "Enter Normal vector values\n"
    "                      x\n"
    "                      y\n"
    "                      z\n";
    drawUI();
    move(5,0);

    char str[80];
    getstr(str);
    float val = atof(str);
    
    getstr(str);
    float val2 = atof(str);

    getstr(str);
    
    onWindow = true;
    
    action = "";
    drawUI();

    pnt3 p = {val, val2, (float)atof(str)};

    screen::getLastScreen()->setNormal(p);
    
    OpenGLhandler::bufferObjects();
    OpenGLhandler::reDraw();
  }
}

