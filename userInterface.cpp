/* 
 * File:   userInterface.cpp
 * Author: btacklind
 * 
 * This file handles the user interface
 */

#include "userInterface.h"
#include "OpenGLhandler.h"
#include "curve2d.h"
#include "screen2d.h"
#include <string>
#include <curses.h>
#include <iostream>
#include "valueHolder.h"

using namespace std;

//int userInterface::objSelected;
char* userInterface::action;
bool userInterface::valueMode;
bool userInterface::isStarted = false;
//bool userInterface::onWindow;
valueHolder* userInterface::vals;
userInterface::mode userInterface::currMode = modify;

void userInterface::init(){
  initscr();
  isStarted = true;
  keypad(stdscr, TRUE);
  start_color();
  init_pair(1, COLOR_RED, COLOR_BLACK);
  action = "";
  if(!has_colors()) action = "Does not support color!";
  else if(!can_change_color()) action = "Can't change color";
//  objSelected = 0;
//  onWindow = true;
  valueMode = false;
  vals = nullptr;
  drawUI();
}

void userInterface::drawUI(){
  clear();
  attron(COLOR_PAIR(1));
  printw("USE [ESC] TO %s, press [h] for help", valueMode?"CANCEL VALUE MODE":"END PROGRAM");
  //printw("\nHAVE FOCUS ON: %s\n", onWindow?"GLUT screen":"Terminal");
  attroff(COLOR_PAIR(1));

  printw("\nCurrent File Loaded: \"%s\"", curve2d::getStoredFile());

  printw("\n");
  printw("\nIn %s mode", getModeStr());
  
  printw("\n");
  printw("\n%s", action);

  if(vals != nullptr) move(5 + vals->getCursorRelative().y, vals->getCursorRelative().x);

  refresh();
}

string userInterface::getModeStr() {
  if(currMode == add   ) return "Add";
  if(currMode == modify) return "Modify";
  if(currMode == remove) return "Remove";
}

void userInterface::printError(char* mes){
  if (!isStarted){
    cout<<mes<<endl;
    return;
  }
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
  if (valueMode){
    if(key == 9 || key == ' ' || key == 13){


      if(!vals->nextVal()){
        valueMode = false;

        action = vals->interpret();
        
        drawUI();

        delete vals;
        vals = nullptr;

        return;
      }

      action = vals->getMessage();

      drawUI();
   
      
      return;
    }
    if(key == 8){
      vals->removeChar();
      action = vals->getMessage();
      drawUI();
      return;
    }

    if(key == 27){
      valueMode = false;
      delete vals;
      vals = nullptr;
      
      action = "";
      drawUI();
      return;
    }
    
    if(key > 31 && key < 127){

      vals->addChar(key);
    
      action = vals->getMessage();
      drawUI();
      delete action;
    }
  }
  else if(key == 'h'){
    clear();
    attron(COLOR_PAIR(1));
    printw("USE [ESC] TO END PROGRAM, press [h] for help");
    attroff(COLOR_PAIR(1));
    printw("\nbracketed letter indicates which to press\n");
    printw("\n[l]oad from file");
    printw("\n[p] save to file");
    
    printw("\n\nAlter Modes:");
//    printw("\n[r] Tgl Draw Mode");
    printw("\n[A]dd mode");
    printw("\n[s] remove mode");
    printw("\nmo[d]dify mode");

    refresh();
  }
  else if(key == 'l'){
    valueMode = true;
    
    char** c = new char*[2]; 
    c[0] = "Enter name of file, or nothing for last file: "; 
    c[1] = "";


    vals = new valueHolder(1, c, "", new interpretLoad());
    action = vals->getMessage();
    drawUI();
    return;
  }
  else if(key == 'p'){
    valueMode = true;
    
    char** c = new char*[2]; 
    c[0] = "Enter name of file, or nothing for last file: "; 
    c[1] = "";


    vals = new valueHolder(1, c, "", new interpretSave());
    action = vals->getMessage();
    drawUI();
    return;
  }/*
  else if(key == ' '){
    screen3d* s = screen3d::getLastScreen();
    s->setNormal(rotateAboutZ(s->getNormal(), 1));

    drawUI();
    OpenGLhandler::bufferObjects();
    OpenGLhandler::reDraw();
  }
  else if(key == 'n'){

    valueMode = true;
    
    char** c = new char*[4]; 
    c[0] = "Enter New view direction: <"; 
    c[1] = ", ";
    c[2] = ", ";
    c[3] = ">";


    vals = new valueHolder(3, c, "0", new interpretNewNormal());
    action = vals->getMessage();
    drawUI();
    return;
  }
  else if(key == 'a'){

    valueMode = true;
    
    char** c = new char*[4]; 
    c[0] = "Enter New Ambient: {"; 
    c[1] = ", ";
    c[2] = ", ";
    c[3] = "}";


    vals = new valueHolder(3, c, "0", new interpretNewPnt3(OpenGLhandler::setAmbiant, "New Ambiant color set"));
    action = vals->getMessage();
    drawUI();
    return;
  }
  else if(key == 'd'){

    valueMode = true;
    
    char** c = new char*[4]; 
    c[0] = "Enter New Diffuse: {"; 
    c[1] = ", ";
    c[2] = ", ";
    c[3] = "}";


    vals = new valueHolder(3, c, "0", new interpretNewPnt3(OpenGLhandler::setDiffuse, "New Diffuse color set"));
    action = vals->getMessage();
    drawUI();
    return;
  }
  else if(key == 's'){

    valueMode = true;
    
    char** c = new char*[4]; 
    c[0] = "Enter New Specular: {"; 
    c[1] = ", ";
    c[2] = ", ";
    c[3] = "}";


    vals = new valueHolder(3, c, "0", new interpretNewPnt3(OpenGLhandler::setSpecular, "New Specular color set"));
    action = vals->getMessage();
    drawUI();
    return;
  }
  else if(key == 'q'){

    valueMode = true;
    
    char** c = new char*[4]; 
    c[0] = "Enter New Light Origin: ("; 
    c[1] = ", ";
    c[2] = ", ";
    c[3] = ")";


    vals = new valueHolder(3, c, "0", new interpretNewPnt3(OpenGLhandler::setLpos, "New light position set"));
    action = vals->getMessage();
    drawUI();
    return;
  }
  else if(key == 't'){

    valueMode = true;
    
    char** c = new char*[4]; 
    c[0] = "Enter New Mega Pixel tone: {"; 
    c[1] = ", ";
    c[2] = ", ";
    c[3] = "}";


    vals = new valueHolder(3, c, "0", new interpretNewPnt3(OpenGLhandler::setMPixTone, "New mega pixel tone set"));
    action = vals->getMessage();
    drawUI();
    return;
  }
  else if(key == 'w'){

    valueMode = true;
    
    char** c = new char*[2]; 
    c[0] = "Enter Light Intensity const: "; 
    c[1] = " ";


    vals = new valueHolder(1, c, "0", new interpretNewFloat(OpenGLhandler::setIl, "New Light Intensity set"));
    action = vals->getMessage();
    drawUI();
    return;
  }
  else if(key == 'e'){

    valueMode = true;
    
    char** c = new char*[2]; 
    c[0] = "Enter Ambiant Intensity const: "; 
    c[1] = " ";


    vals = new valueHolder(1, c, "0", new interpretNewFloat(OpenGLhandler::setIa, "New Ambiant Intensity set"));
    action = vals->getMessage();
    drawUI();
    return;
  }
  else if(key == 'k'){

    valueMode = true;
    
    char** c = new char*[2]; 
    c[0] = "Set Average Light Distance: "; 
    c[1] = " ";


    vals = new valueHolder(1, c, "0", new interpretNewFloat(OpenGLhandler::setK, "New Average Light Distance set"));
    action = vals->getMessage();
    drawUI();
    return;
  }
  else if(key == 'z'){

    valueMode = true;
    
    char** c = new char*[2]; 
    c[0] = "Interpret light size: "; 
    c[1] = " ";


    vals = new valueHolder(1, c, "0", new interpretLightSize());
    action = vals->getMessage();
    drawUI();
    return;
  }
  else if(key == 'r'){
    OpenGLhandler::tglDrawMode();
    
    OpenGLhandler::bufferObjects();
    OpenGLhandler::reDraw();
  }
  else if(key == 'x'){
    OpenGLhandler::tglPixMode();
    
    OpenGLhandler::bufferObjects();
    OpenGLhandler::reDraw();
  }/*
  else if(key == 'x'){
    OpenGLhandler::tglLightMode();
    
    OpenGLhandler::bufferObjects();
    OpenGLhandler::reDraw();
  }*/
}


void userInterface::leftMouseClick(int x, int y, bool ButtonDown){
  if(y > getHelpHeight()){
    if(ButtonDown){
      screen2d* s = screen2d::getMainScreen();

      if(currMode == add){
        pixelSelectionHelper h = s->getNearestLine(x, OpenGLhandler::getScreenHeight() - getHelpHeight() - y);

        if(h.distance < clickDistance){
          h.c->addPoint(h.index, s->translate(x, OpenGLhandler::getScreenHeight() - getHelpHeight() - y));
          s->draw();

          currMode = modify;

          selectedCurve = h.c;
          selectedPoint = h.index;
          setTime();

          valueMode = true;
          char** c = new char*[3]; 
          c[0] = "new position ("; 
          c[1] = ", "; 
          c[2] = ")";
          vals = new valueHolder(2, c, "0.0", new interpretNewAddPoint(h.c, h.index, "New position set"));

          return;
        }
      }
      else{
        pixelSelectionHelper h = s->getNearestPoint(x, OpenGLhandler::getScreenHeight() - getHelpHeight() - y);

        if(h.distance < clickDistance){
          if(currMode == modify){
            selectedCurve = h.c;
            selectedPoint = h.index;
            setTime();

            valueMode = true;
            char** c = new char*[3]; 
            c[0] = "new position ("; 
            c[1] = ", "; 
            c[2] = ")";
            vals = new valueHolder(2, c, "0.0", new interpretNewAddPoint(h.c, h.index, "New position set"));

            return;
          }
          else  if(currMode == remove){
            h.c->removePoint(h.index);
            s->draw();
          }
        }

        selectedCurve = nullptr;
        selectedPoint = 0;
      }
    }
    else{
      if(getTime() > holdTime){
        selectedCurve = nullptr;
        selectedPoint = 0;

        if(valueMode){
          valueMode = false;
          delete vals;
          vals = nullptr;
        }
      }
    }
  }
}

unsigned long int userInterface::getTime() {
  timeval tv;
  gettimeofday(&tv, 0);
  return tv.tv_usec - lastTime;
}

void userInterface::setTime() {
  timeval tv;
  gettimeofday(&tv, 0);
  lastTime = tv.tv_usec;
}

void userInterface::mouseMove(int x, int y) {
  if(selectedCurve == nullptr) return;
  
  screen2d* s = screen2d::getMainScreen();
  if(currMode == modify){
    selectedCurve->modifyPoint(selectedPoint, s->translate(x, OpenGLhandler::getScreenHeight() - getHelpHeight() - y));  
    s->draw();
  }
}

char* userInterface::interpretNewAddPoint::operator()(char** c) {
  pntf p = {(float)atof(c[0]), (float)atof(c[1])};
  curve->addPoint(index, p);
  return mes;
}


char* userInterface::interpretLoad::operator()(char** c){
  if(c[0][0] == 0) {
    if(curve2d::load());
      return "Successful load";
  }
  else{
    if(curve2d::load(c[0]))
      return "Successful load";
  }
  return "failed load!";
}

char* userInterface::interpretSave::operator()(char** c){
  if(c[0][0] == 0) {
    if(curve2d::save());
      return "Successful save";
  }
  else{
    if(curve2d::save(c[0]))
      return "Successful save";
  }
  return "failed save!";
}

/*
char* userInterface::interpretNewNormal::operator()(char** c){
  pnt3 p = {(float)atof(c[0]), (float)atof(c[1]), (float)atof(c[2])};

  if (p == zeroVector) return "Can't use zero Vector as normal";

  screen3d::getLastScreen()->setNormal(p);
  
  OpenGLhandler::bufferObjects();
  OpenGLhandler::reDraw();

  return "normal set";
}

char* userInterface::interpretNewPnt3::operator()(char** c){
  pnt3 p = {(float)atof(c[0]), (float)atof(c[1]), (float)atof(c[2])};

  setter(p);
  
  OpenGLhandler::bufferObjects();
  OpenGLhandler::reDraw();

  return (char*)mes;
}

char* userInterface::interpretNewFloat::operator()(char** c){
  setter((float)atof(c[0]));
  
  OpenGLhandler::bufferObjects();
  OpenGLhandler::reDraw();

  return (char*)mes;
}

char* userInterface::interpretLightSize::operator()(char** c){
  OpenGLhandler::setLightSize((unsigned int)atoi(c[0]));

  OpenGLhandler::bufferObjects();
  OpenGLhandler::reDraw();

  return "New Light size set";
}
*/
