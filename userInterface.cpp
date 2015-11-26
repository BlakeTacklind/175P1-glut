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
userInterface::mode userInterface::currMode = Modify;
curve2d* userInterface::selectedCurve = nullptr;
unsigned int userInterface::selectedPoint = 0;
unsigned long int userInterface::lastTime;
bool userInterface::isMoving = false;
bool userInterface::isMaximum = false;
userInterface::DrawMode userInterface::dMode = both;

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
  if(!isStarted) return;

  clear();
  attron(COLOR_PAIR(1));
  printw("USE [ESC] TO %s, press [h] for help", valueMode?"CANCEL VALUE MODE":"END PROGRAM");
  //printw("\nHAVE FOCUS ON: %s\n", onWindow?"GLUT screen":"Terminal");
  attroff(COLOR_PAIR(1));

  printw("\nCurrent File Loaded: \"%s\"", curve2d::getStoredFile());

  printw("\n");
  printw("\nResolution %u %s", OpenGLhandler::getResolution(), isMaximum?"(Maximized)":"");

  printw("\n");
  printw("\nIn %s mode", getModeStr());
  printw("\nDrawing %s", getDrawStr());
  
  printw("\n");
  printw("\n%s", action);

  if(vals != nullptr) move(8 + vals->getCursorRelative().y, vals->getCursorRelative().x);

  refresh();
}

char* userInterface::getModeStr() {
  if(currMode == Add   ) return "Add";
  if(currMode == Modify) return "Modify";
  if(currMode == Remove) return "Remove";
  if(currMode == UMod  ) return "U modification";
}

char* userInterface::getDrawStr() {
  if(dMode == curves ) return "Curves";
  if(dMode == both   ) return "Both Control Lines and Curves";
  if(dMode == control) return "Control lines";
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
    
    printw("\nSet [R]esolution");
    printw("\nMaximize R[e]solution");
    printw("\n\nAlter Modes:");
    printw("\n[q] Tgl Draw Mode");
    printw("\n[A]dd mode");
    printw("\n[s] remove mode");
    printw("\nmo[d]dify mode");

    printw("\n[f] Set k if b-Spline Selected");

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
  }
  else if(key == 'a'){
    currMode = Add;
    drawUI();
    return;
  }
  else if(key == 's'){
    currMode = Remove;
    drawUI();
    return;
  }
  else if(key == 'd'){
    currMode = Modify;
    drawUI();
    return;
  }
  else if(key == 'r'){
    valueMode = true;
    
    char** c = new char*[2]; 
    c[0] = "Set Resolution to: "; 
    c[1] = "";


    vals = new valueHolder(1, c, "", new interpretNewUInt(OpenGLhandler::setResolution, "new resolution set"));
    action = vals->getMessage();
    drawUI();
    return;
  }
  else if(key == 'e'){
    if(!drawCurves()) dMode = both;

    while(!isMaximum){
      OpenGLhandler::setResolution(OpenGLhandler::getResolution() + 1);
      screen2d::drawAll();
      OpenGLhandler::reDraw();
      drawUI();
    }
  }
  else if(key == 'q'){
         if(dMode == curves ) dMode = both;
    else if(dMode == both   ) dMode = control;
    else if(dMode == control) dMode = curves;

    screen2d::drawAll();
    OpenGLhandler::reDraw();
    drawUI();
  }
  else if(key == 'f'){
    if(selectedCurve->getCurveType() != BSpline){
      printError("Not a B-Spline Selected");
      return;
    }

    bSpline* b = (bSpline*)selectedCurve;

    char** c = new char*[2]; 
    c[0] = "Set k to: "; 
    c[1] = "";

    vals = new valueHolder(1, c, "0", new interpretNewK(b, "new k set"));
    action = vals->getMessage();
    screen2d::drawAll();
    OpenGLhandler::reDraw();
    drawUI();
  }
  /*
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
  
  if(ButtonDown){
    if(valueMode){
      valueMode = false;
      delete vals;
      vals = nullptr;
      action = "";
      drawUI();
    }

    if(y > getHelpHeight()){
      screen2d* s = screen2d::getMainScreen();
      if(currMode == UMod){
        currMode = Modify;
        drawUI();
      }

      if(currMode == Add){
        pixelSelectionHelper h = s->getNearestLine(x, OpenGLhandler::getScreenHeight() - y);

        if(h.distance < clickDistance){
          h.c->addPoint(h.index, s->translate(x, OpenGLhandler::getScreenHeight() - y));
          s->draw();

          // currMode = Modify;

          selectedCurve = h.c;
          selectedPoint = h.index;
          setTime();

          isMoving = true;
          screen2d::drawAll();
          OpenGLhandler::reDraw();

          return;
        }
        else{
          selectedCurve = h.c;
          screen2d::drawAll();
          OpenGLhandler::reDraw();
        }
      }
      else{
        pixelSelectionHelper h = s->getNearestPoint(x, OpenGLhandler::getScreenHeight() - y);
        // cout<<"x "<<x<<" y "<<OpenGLhandler::getScreenHeight() - y<<" dist "<<h.distance<<endl;

        if(h.distance < clickDistance){
          if(currMode == Modify){

            selectedCurve = h.c;
            selectedPoint = h.index;

            isMoving = true;

            setTime();
            screen2d::drawAll();
            OpenGLhandler::reDraw();

            return;
          }
          else  if(currMode == Remove){
            selectedCurve = h.c;
            h.c->removePoint(h.index);
            screen2d::drawAll();
            OpenGLhandler::reDraw();
          }
        }
        else{
          h = s->getNearestLine(x, OpenGLhandler::getScreenHeight() - y);
          selectedCurve = h.c;
          screen2d::drawAll();
          OpenGLhandler::reDraw();
        }
      }
    }
    else{
      if(selectedCurve->getCurveType() == curve2d::BSpline){
        selectedPoint = ((bSpline*)selectedCurve)->getUNum(screen2d::getHelpScreen()->translateX(x));
        currMode = UMod;
        setTime();
        isMoving = true;

        // cout<<"test "<<selectedPoint<<endl;

        drawUI();
      }
    }
  }
  else{
    isMoving = false;
  
    if(getTime() < holdTime){


      if(currMode == Modify){
        char** c = new char*[3]; 
        c[0] = "new position for point ("; 
        c[1] = ", "; 
        c[2] = ")";
        vals = new valueHolder(2, c, "0", new interpretNewModPoint(selectedCurve, selectedPoint, "New position set for point"));
      }
      else if(currMode == Add){
        char** c = new char*[3]; 
        c[0] = "new position for new point ("; 
        c[1] = ", "; 
        c[2] = ")";
        vals = new valueHolder(2, c, "0", new interpretNewAddPoint(selectedCurve, selectedPoint, "New point's position set"));
      }
      else if(currMode == UMod){
        char** c = new char*[2]; 
        c[0] = "new knot value: "; 
        c[1] = ""; 
        vals = new valueHolder(1, c, "0", new interpretNewUModPoint((bSpline*)selectedCurve, selectedPoint, "New U set"));
      }
      else return;

      valueMode = true;
      action = vals->getMessage();
      drawUI();
    }
  }
}

unsigned long int userInterface::getTime() {
  timeval tv;
  gettimeofday(&tv, 0);
  return ((tv.tv_sec*1000)+(tv.tv_usec/1000.0))+.5 - lastTime;
}

void userInterface::setTime() {
  timeval tv;
  gettimeofday(&tv, 0);
  lastTime = ((tv.tv_sec*1000)+(tv.tv_usec/1000.0))+.5;
  // cout<<"time: "<<lastTime<<endl;
}

void userInterface::mouseMove(int x, int y) {
  if(!isMoving) return;
  if(selectedCurve == nullptr) return;
  if(getTime() < holdTime) return;



  if(currMode == Modify || currMode == Add){
    screen2d* s = screen2d::getMainScreen();
    selectedCurve->modifyPoint(selectedPoint, s->translate(x, OpenGLhandler::getScreenHeight() - y));  
    screen2d::drawAll();
    OpenGLhandler::reDraw();
  }
  else if(currMode == UMod){
    screen2d* s = screen2d::getHelpScreen();
    float f = s->translateX(x);
    bSpline* b = ((bSpline*)selectedCurve);
    if(selectedPoint != 0 && f < b->getU(selectedPoint-1)) f = b->getU(selectedPoint-1);
    if(selectedPoint != b->getNumU() && f > b->getU(selectedPoint+1)) f = b->getU(selectedPoint+1);

    b->modifyU(selectedPoint, f);

    screen2d::drawAll();
    OpenGLhandler::reDraw();
  }
}

char* userInterface::interpretNewAddPoint::operator()(char** c) {
  pntf p = {(float)atof(c[0]), (float)atof(c[1])};
  curve->addPoint(index, p);

  screen2d::drawAll();
  OpenGLhandler::reDraw();

  return (char*)mes;
}

char* userInterface::interpretNewModPoint::operator()(char** c) {
  pntf p = {(float)atof(c[0]), (float)atof(c[1])};
  curve->modifyPoint(index, p);

  screen2d::drawAll();
  OpenGLhandler::reDraw();

  return (char*)mes;
}

char* userInterface::interpretNewUModPoint::operator()(char** c) {
  float p = (float)atof(c[0]);

  if(index != 0)
    if(curve->getU(index-1) > p)
      return "Can't change knot to less then previous knot";
  
  if(index != (curve->getK()+curve->getNumControlPoints()-1))
    if(curve->getU(index+1) < p)
      return "Can't change knot to greater then next knot";

  curve->modifyU(index, p);

  screen2d::drawAll();
  OpenGLhandler::reDraw();

  return (char*)mes;
}


char* userInterface::interpretLoad::operator()(char** c){
  if(c[0][0] == 0) {
    if(curve2d::load()){
      screen2d::drawAll();
      OpenGLhandler::reDraw();
      return "Successful load";
    }
  }
  else{
    if(curve2d::load(c[0])){
      screen2d::drawAll();
      OpenGLhandler::reDraw();
      return "Successful load";
    }
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

char* userInterface::interpretNewUInt::operator()(char** c){
  setter((unsigned int)atoi(c[0]));
  screen2d::drawAll();
  OpenGLhandler::reDraw();
  return (char*)mes;
}

char* userInterface::interpretNewK::operator()(char** c){
  curve->setK((unsigned int)atoi(c[0]));
  screen2d::drawAll();
  OpenGLhandler::reDraw();
  return (char*)mes;
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

char* userInterface::interpretLightSize::operator()(char** c){
  OpenGLhandler::setLightSize((unsigned int)atoi(c[0]));

  OpenGLhandler::bufferObjects();
  OpenGLhandler::reDraw();

  return "New Light size set";
}
*/
