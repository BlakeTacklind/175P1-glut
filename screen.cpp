/* 
 * File:   screen.cpp
 * Author: btacklind
 * 
 * Created on October 20, 2015, 12:36 PM
 */

#include "screen.h"

screen::screen(int x, int y, pnt3 vec, void (*mkPix)(int, int)) {
  width = x;
  height = y;
  vector = vec;
  MakePix = mkPix;
}

screen::screen(const screen& orig) {
}

screen::~screen() {
}

void screen::displayObjects() {
  //get 3d object edges
  //convert to 2d lines float lines
  
  //find mins and maxes
  
  //scale and translate lines to fill screen
  
  //convert float lines to int lines
  
  //display lines
}
