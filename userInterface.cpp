/* 
 * File:   userInterface.cpp
 * Author: btacklind
 * 
 * This file handles the user interface
 */

#include "userInterface.h"
#include "OpenGLhandler.h"
#include "object3Dsurface.h"
#include "screen.h"
#include <string>
#include <curses.h>
#include <iostream>
#include "valueHolder.h"

using namespace std;

int userInterface::objSelected;
char* userInterface::action;
bool userInterface::valueMode;
bool userInterface::isStarted = false;
bool userInterface::onWindow;
valueHolder* userInterface::vals;

void userInterface::init(){
  initscr();
  isStarted = true;
  keypad(stdscr, TRUE);
  start_color();
  init_pair(1, COLOR_RED, COLOR_BLACK);
  action = "";
  if(!has_colors()) action = "Does not support color!";
  else if(!can_change_color()) action = "Can't change color";
  objSelected = 0;
  onWindow = true;
  valueMode = false;
  vals = nullptr;
  drawUI();
}

void userInterface::drawUI(){
  clear();
  attron(COLOR_PAIR(1));
  printw("USE [ESC] TO %s, press [h] for help", valueMode?"CANCEL VALUE MOD":"END PROGRAM");
  //printw("\nHAVE FOCUS ON: %s\n", onWindow?"GLUT screen":"Terminal");
  attroff(COLOR_PAIR(1));

  printw("\nCurrent File Loaded: \"%s\"", object3Dsurface::getStoredFile());
  printw("\n");
  printw("\nLighting Settings:");

  printw("\n");
  pnt3 t = OpenGLhandler::getLpos();
  printw("\nLight Postion: (%f, %f, %f)", t.x, t.y, t.z);
  printw("\nAverage Light Distance (K): %f", OpenGLhandler::getK());
  printw("\nLight Size (n): %u", OpenGLhandler::getLightSize());

  printw("\n");
  t = OpenGLhandler::getMPixTone();
  printw("\nMega Pixel Tone: {%f, %f, %f}", t.x, t.y, t.z);

  printw("\n");
  t = OpenGLhandler::getAmbiant();
  printw("\nAmbient Light: {%f, %f, %f}", t.x, t.y, t.z);
  printw(" with intensity %f", OpenGLhandler::getIa());

  t = OpenGLhandler::getDiffuse();
  printw("\nDiffuse Light: {%f, %f, %f}", t.x, t.y, t.z);
  t = OpenGLhandler::getSpecular();
  printw("\nSpectral Light: {%f, %f, %f}", t.x, t.y, t.z);
  printw("\nLight intensity %f", OpenGLhandler::getIl());


  printw("\n");
  t = screen::getLastScreen()->getNormal();
  printw("\n4th quadrant view normal: <%f, %f, %f>", t.x, t.y, t.z);

  printw("\n");
  printw("\n%s", action);

  if(vals != nullptr) move(18 + vals->getCursorRelative().y, vals->getCursorRelative().x);

  refresh();
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
    printw("\n[l]oad from [f]ile");
    printw("\n[space] for rotate oblique window about z-axis");
    printw("\n[n] Set normal for oblique window");
    printw("\n\nAlter Modes:");
    printw("\n[x] Tgl Mega Pix");
    printw("\n[r] Tgl Draw Mode");
    //printw("\n[x] Algorithms");
    printw("\n\nLighting Settings:");
    printw("\n[A]mbient");
    printw("\n[D]iffuse");
    printw("\n[S]pecular");
    printw("\n[K] Average Light Distance");
    printw("\n[q] Light position");
    printw("\n[w] Light Intensity");
    printw("\n[e] Ambient Intensity");
    printw("\n[z] Light Size");
    printw("\n[t] Set Mega Pix tone");

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
  else if(key == ' '){
    screen* s = screen::getLastScreen();
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



char* userInterface::interpretLoad::operator()(char** c){
  if(c[0][0] == 0) {
    if(object3Dsurface::load());
      return "Successful load";
  }
  else{
    if(object3Dsurface::load(c[0]))
      return "Successful load";
  }
  return "failed load!";
}

char* userInterface::interpretNewNormal::operator()(char** c){
  pnt3 p = {(float)atof(c[0]), (float)atof(c[1]), (float)atof(c[2])};

  if (p == zeroVector) return "Can't use zero Vector as normal";

  screen::getLastScreen()->setNormal(p);
  
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
